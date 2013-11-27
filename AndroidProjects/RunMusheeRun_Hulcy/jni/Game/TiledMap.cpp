#include "TiledMap.h"
#include "CoreEngine/Util/StringUtils.h"
#include "CoreEngine/Util/Base64.h"
#include "MonkyRenderer/GLBuffer.h"
#include "MonkyRenderer/Texture.h"
#include "MonkyRenderer/Material.h"
#include "MonkyRenderer/BufferLayout.h"
#include "MonkyRenderer/VtxBufferAccessor.h"
#include "MonkyRenderer/Mesh.h"

namespace Monky
{

	TiledMap::TiledMap( const std::string& mapFile )
		: 	m_mapFile( mapFile )
	{
		XMLParser parser( mapFile.c_str() );
		XMLDocument& doc = parser.getDocument();

		if( !doc.Error() )
		{
			XMLNode* map = doc.FirstChildElement( "map" );
			m_width = parser.getXMLAttributeAsInt( map, "width", 0 );
			m_height = parser.getXMLAttributeAsInt( map, "height", 0 );
			m_tileWidth = parser.getXMLAttributeAsInt( map, "tilewidth", 0 );
			m_tileHeight = parser.getXMLAttributeAsInt( map, "tileheight", 0 );

			LoadTileSets( parser, map );
			consolePrintf( "All tilesets loaded..." );
			LoadTileLayers( parser, map );
			consolePrintf( "All Tile layers loaded..." );
			ConstructTileLayerMeshes();
			consolePrintf( "All meshes constructed..." );
		}
	}

	void TiledMap::RenderMap()
	{
		NamedProperties params;
		matStackf matStack;
		matStack.loadIdentity();
		params.set( "modelMatrix", matStack );
		for( int i = 0; i < m_layers.size(); ++i )
		{
			for( auto iter = m_layers[i].tileGroups.begin(); iter != m_layers[i].tileGroups.end(); ++iter )
			{
				params.set( "mesh", iter->second.mesh );
				fireEvent( "renderMesh", params );
				consolePrintf( "LayerGroup Rendered" );
			}
		}
	}

	void TiledMap::LoadTileSets( XMLParser& parser, XMLNode* root )
	{
		XMLNode* tilesetNode = root->FirstChildElement( "tileset" );
		for( ; tilesetNode != nullptr; tilesetNode = tilesetNode->NextSiblingElement( "tileset" ) )
		{
			int firstGid = parser.getXMLAttributeAsInt( tilesetNode, "firstgid", 1 );
			std::string name = parser.getXMLAttributeAsString( tilesetNode, "name", "" );
			int tileWidth = parser.getXMLAttributeAsInt( tilesetNode, "tilewidth", 0 );
			int tileHeight = parser.getXMLAttributeAsInt( tilesetNode, "tileheight", 0 );
			XMLNode* imageSrc = tilesetNode->FirstChildElement( "image" );
			std::string source = parser.getXMLAttributeAsString( imageSrc, "source", "" );
			int srcWidth = parser.getXMLAttributeAsInt( imageSrc, "width", 0 );
			int srcHeight = parser.getXMLAttributeAsInt(imageSrc, "height", 0 );

			std::vector< std::string > sourcePath;
			stringTokenizer( source, sourcePath, "/" );

			m_tileSets.push_back( TileSet( firstGid, name, tileWidth, tileHeight, sourcePath[ sourcePath.size() - 1], srcWidth, srcHeight ) );
		}
	}

	void TiledMap::LoadTileLayers( XMLParser& parser, XMLNode* root )
	{
		XMLNode* layerNode = root->FirstChildElement( "layer" );
		for( ; layerNode != nullptr; layerNode = layerNode->NextSiblingElement( "layer" ) )
		{
			std::string name = parser.getXMLAttributeAsString( layerNode, "name", "" );

			int width = parser.getXMLAttributeAsInt( layerNode, "width", 0 );
			int height = parser.getXMLAttributeAsInt( layerNode, "height", 0 );
			parser.validateXMLChildElements( layerNode, "data", "" );

			consolePrintf( "Loading layer: %s (%d,%d)", name.c_str(), width, height );

			XMLNode* dataNode = layerNode->FirstChildElement( "data" );
			std::string encoding = parser.getXMLAttributeAsString( dataNode, "encoding", "" );
			if( encoding == "base64" )
			{
				std::string encodedData = parser.getXMLElementPCDataAsString( dataNode );
				consolePrintf( "Encoded data: %d", encodedData.size() );

				stripLeadingAndTrailingWhiteSpace( encodedData );

				std::vector<int> decodedData = base64_decode( encodedData );

				m_layers.push_back( TileLayer() );

				TileLayer& newLayer = m_layers[ m_layers.size() - 1 ];
				newLayer.name = name;
				newLayer.width = width;
				newLayer.height = height;

				int size = decodedData.size();
				consolePrintf( "Decoded data size: %d", size );
				for( int i = 0; i < size; i += 4 )
				{
					int gid = decodedData[i] |
							decodedData[i + 1] << 8 |
							decodedData[i + 2] << 16 |
							decodedData[i + 3] << 24;

					TileSet* tileset = GetTileSetGIDIsIn( gid );


					if( tileset != nullptr )
					{
						consolePrintf( "Creating tile with gid: %d", gid );
						Tile tile;
						tile.tileSet = tileset;
						tile.gid = tileset->GetNormalizedGIDInTileSet( gid );
						tile.pos = GetLocationFromIndex( i / 4 );
						newLayer.tileGroups[ tileset->GetName() ].tiles.push_back( tile );
					}
				}

			}
			else
			{
				consolePrintf( "Invalid map encoding for: %s", m_mapFile.c_str() );
			}

		}
	}

	TileSet* TiledMap::GetTileSetGIDIsIn( int gid )
	{
		TileSet* tileSetGIDIsIn = nullptr;
		for( int i = 0; i < m_tileSets.size() && tileSetGIDIsIn == nullptr; ++i )
		{
			if( m_tileSets[i].IsInTileSet( gid ) )
				tileSetGIDIsIn = &m_tileSets[i];
		}
		return tileSetGIDIsIn;
	}

	void TiledMap::ConstructTileLayerMeshes()
	{
		for( int i = 0; i < m_layers.size(); ++i )
		{
			for( auto iter = m_layers[0].tileGroups.begin(); iter != m_layers[0].tileGroups.end(); ++iter )
			{
				int numTiles = iter->second.tiles.size();
				int numVertices = numTiles * 4;
				std::vector< MapVertex > vertices;
				std::vector< unsigned int > indices;

				for( size_t i = 0; i < numTiles; ++i )
				{

					vec2f texCoordSize = iter->second.tiles[i].tileSet->GetTileSizeInUVSpace();


					vec3f tilePos = iter->second.tiles[i].pos;
					vec2f topLeftTexCoord = iter->second.tiles[i].tileSet->GetTopLeftTexCoords( iter->second.tiles[i].gid );

					vertices.push_back( MapVertex( vec3f( tilePos.x - m_tileWidth * 0.5f, tilePos.y + m_tileWidth * 0.5f, tilePos.z ),
										vec2f( topLeftTexCoord.x, topLeftTexCoord.y ) ) );

					vertices.push_back( MapVertex( vec3f( tilePos.x + m_tileWidth * 0.5f, tilePos.y + m_tileWidth * 0.5f, tilePos.z ),
										topLeftTexCoord + vec2f( texCoordSize.x , 0.0f ) ) );

					vertices.push_back( MapVertex( vec3f( tilePos.x + m_tileWidth * 0.5f, tilePos.y - m_tileWidth * 0.5f, tilePos.z ),
										topLeftTexCoord + texCoordSize ) );

					vertices.push_back( MapVertex( vec3f( tilePos.x - m_tileWidth * 0.5f, tilePos.y - m_tileWidth * 0.5f, tilePos.z ),
										topLeftTexCoord + vec2f( 0.0f, texCoordSize.y ) ) );

					indices.push_back( 4*i );
					indices.push_back( 4*i + 2 );
					indices.push_back( 4*i + 3 );
					indices.push_back( 4*i + 2 );
					indices.push_back( 4*i );
					indices.push_back( 4*i + 1 );
				}

				BufferLayout* vtxBufferLayout = new BufferLayout( 3,
							BufferLayout::FLOAT3,
							BufferLayout::FLOAT4,
							BufferLayout::FLOAT2 );
				size_t vtxBufferSize = vtxBufferLayout->stride() * vertices.size();
				char* vtxBufferData = new char[ vtxBufferSize ];
				std::memcpy( vtxBufferData, (char*)vertices.data(), vtxBufferSize );

				BufferLayout* idxBufferLayout = new BufferLayout( 1, BufferLayout::INT );
				size_t idxBufferSize = idxBufferLayout->stride() * indices.size();
				char* idxBufferData = new char[ idxBufferSize ];
				std::memcpy( idxBufferData, (char*)indices.data(), idxBufferSize );


				GLBuffer* vtxBuffer = new GLBuffer( vtxBufferData, vertices.size(), vtxBufferLayout, GL_ARRAY_BUFFER );
				GLBuffer* idxBuffer = new GLBuffer( idxBufferData, indices.size(), idxBufferLayout, GL_ELEMENT_ARRAY_BUFFER );

				vtxBuffer->sendToOpenGL();
				idxBuffer->sendToOpenGL();

				std::map< VtxBufferAccessor::AttributeType, int > vtxMapping;
				vtxMapping[ VtxBufferAccessor::POSITION ] = 0;
				vtxMapping[ VtxBufferAccessor::COLOR ] = 1;
				vtxMapping[ VtxBufferAccessor::UV ] = 2;
				VtxBufferAccessor* vtxAccessor = new VtxBufferAccessor( vtxBuffer, vtxMapping );

				iter->second.mesh  = new Mesh( vtxBuffer, idxBuffer, vtxAccessor, iter->second.tiles[0].tileSet->GetMaterial() );
			}

		}
	}

	vec3f TiledMap::GetLocationFromIndex( int i )
	{
		int xIdx = i % m_width;
		int yIdx = (int) ( i / (float)m_height );
		return vec3f( xIdx * m_tileWidth, yIdx * m_tileHeight );
	}


}
