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

	// Bits on the far end of the 32-bit global tile ID are used for tile flags
	const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
	const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

	Mesh* GenerateObjBoxDebugMesh( const aabb2f& box );

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
			LoadTileLayers( parser, map );
			ProcessObjectGroups( parser, map );
			ConstructTileLayerMeshes();


		}
	}

	void TiledMap::RenderMap( bool shouldShowDebug )
	{
		NamedProperties params;
		matStackf matStack;
		matStack.loadIdentity();
		matStack.translate( (float)-m_width, (float)-m_height, 0.0f );
		params.set( "modelMatrix", matStack.top() );
		for( int i = 0; i < m_layers.size(); ++i )
		{
			for( auto iter = m_layers[i].tileGroups.begin(); iter != m_layers[i].tileGroups.end(); ++iter )
			{
				params.set( "mesh", iter->second.mesh );
				fireEvent( "renderMesh", params );
			}
		}

		if( shouldShowDebug )
		{
			matStack.loadIdentity();
			for( auto iter = m_objects.begin(); iter != m_objects.end(); ++iter )
			{
				params.set( "mesh", iter->second.mesh );
				fireEvent( "renderMesh", params );
			}
			params.set( "mesh", m_checkpoints.front().mesh );
		}
	}

	void TiledMap::ReloadLayers()
	{
		for( int i = 0; i < m_layers.size(); ++i )
		{
			for( auto iter = m_layers[i].tileGroups.begin(); iter != m_layers[i].tileGroups.end(); ++iter )
			{
				iter->second.mesh->reloadBuffers();
			}
		}
	}


	bool TiledMap::DidPlayerCollideWithCheckpoint( const aabb2f& playerBox, vec2f& checkpointPos )
	{
		bool collidedWithCheckpoint = false;
		if( m_checkpoints.size() > 0 )
		{
			vec2f intersectionDist;
			if( playerBox.intersects( m_checkpoints.front().box, intersectionDist ) )
			{
				collidedWithCheckpoint = true;
				checkpointPos = m_checkpoints.front().box.getCenter();
				m_checkpoints.pop();
			}
		}
		return collidedWithCheckpoint;
	}

	bool TiledMap::DidPlayerCollideWithBottomBounds( const aabb2f& playerBox, vec2f& overlapDist )
	{
		bool collidedWith = false;

		vec2f intersectionDist;
		vec2f botMinDist = m_objects[ "bottom" ].boundingBox.minPoint();
		intersectionDist.y = botMinDist.y - playerBox.maxPoint().y;
		//if( intersectionDist.y < 0 )
		if( playerBox.intersects( m_objects[ "bottom" ].boundingBox, intersectionDist ) )
		{
			collidedWith = true;
			overlapDist = intersectionDist;
			consolePrintf( "Collided with bottom" );
			consolePrintf( "Player box: %s", playerBox.toString().c_str() );
			consolePrintf( "Bot box: %s", m_objects[ "bottom" ].boundingBox.toString().c_str() );
		}

		return collidedWith;
	}

	bool TiledMap::DidPlayerCollideWithTopBounds( const aabb2f& playerBox, vec2f& overlapDist )
	{
		bool collidedWith = false;

		vec2f intersectionDist;
		vec2f topMaxDist = m_objects[ "top" ].boundingBox.maxPoint();
		intersectionDist.y = playerBox.minPoint().y - topMaxDist.y;
		//if( intersectionDist.y < 0 )
		if( playerBox.intersects( m_objects[ "bottom" ].boundingBox, intersectionDist ) )
		{
			collidedWith = true;
			overlapDist = intersectionDist;
			consolePrintf( "Collided with top" );
			consolePrintf( "Player box: %s", playerBox.toString().c_str() );
			consolePrintf( "Top box: %s", m_objects[ "top" ].boundingBox.toString().c_str() );
		}

		return collidedWith;
	}

	bool TiledMap::DidPlayerCollideWithBounds( const aabb2f& playerBox, vec2f& overlapDist )
	{
		bool collidedWith = false;

		collidedWith = DidPlayerCollideWithTopBounds( playerBox, overlapDist );
		if( !collidedWith )
		{
			collidedWith = DidPlayerCollideWithBottomBounds( playerBox, overlapDist );
		}
		return collidedWith;
	}

	bool TiledMap::DidPlayerCollideWithKillBox( const aabb2f& playerBox )
	{
		bool collidedWith = false;
		vec2f intersectionDist;
		if( playerBox.intersects( m_objects[ "killbox" ].boundingBox, intersectionDist ) )
		{
			collidedWith = true;
		}
		return collidedWith;
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
				int tile_index = 0;

				for( int y = 0; y < m_height; ++y )
					for( int x = 0; x < m_width; ++x )
					{
						int gid = decodedData[tile_index] |
								decodedData[tile_index + 1] << 8 |
								decodedData[tile_index + 2] << 16 |
								decodedData[tile_index + 3] << 24;

						tile_index +=4;

						gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
								 FLIPPED_VERTICALLY_FLAG |
								 FLIPPED_DIAGONALLY_FLAG);

						TileSet* tileset = GetTileSetGIDIsIn( gid );


						if( tileset != nullptr )
						{
							consolePrintf( "Creating tile with gid: %d", gid );
							Tile tile;
							tile.tileSet = tileset;
							tile.gid = tileset->GetNormalizedGIDInTileSet( gid );
							int tileYFlipped = y;//m_height - y;
							tile.pos = vec3f( (float)x*m_tileWidth, (float)tileYFlipped*m_tileHeight, 0.0f );
							//consolePrintf( "Pos: %s", tile.pos.toString().c_str() );
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
				std::vector< Mesh::Vertex > vertices;
				std::vector< unsigned int > indices;

				for( int i = 0; i < numTiles; ++i )
				{

					vec2f texCoordSize = iter->second.tiles[i].tileSet->GetTileSizeInUVSpace();


					vec3f tilePos = iter->second.tiles[i].pos;
					vec2f topLeftTexCoord = iter->second.tiles[i].tileSet->GetTopLeftTexCoords( iter->second.tiles[i].gid );

					vertices.push_back( Mesh::Vertex( vec3f( tilePos.x, tilePos.y + m_tileHeight, tilePos.z ),
													vec3f( 0.0f, 0.0f, 1.0f ),
													color::WHITE,
													topLeftTexCoord + vec2f( 0.0f, texCoordSize.y ) ) );

					//consolePrintf( "TexCoords: %s", vertices[ vertices.size() - 1 ].texCoords.toString().c_str() );

					vertices.push_back( Mesh::Vertex( vec3f( tilePos.x + m_tileWidth, tilePos.y + m_tileHeight, tilePos.z ),
													vec3f( 0.0f, 0.0f, 1.0f ),
													color::WHITE,
													topLeftTexCoord + texCoordSize ) );

					//consolePrintf( "TexCoords: %s", vertices[ vertices.size() - 1 ].texCoords.toString().c_str() );

					vertices.push_back( Mesh::Vertex( vec3f( tilePos.x + m_tileWidth, tilePos.y, tilePos.z ),
													vec3f( 0.0f, 0.0f, 1.0f ),
													color::WHITE,
													topLeftTexCoord + vec2f( texCoordSize.x , 0.0f ) ) );


					//consolePrintf( "TexCoords: %s", vertices[ vertices.size() - 1 ].texCoords.toString().c_str() );

					vertices.push_back( Mesh::Vertex( vec3f( tilePos.x, tilePos.y, tilePos.z ),
													vec3f( 0.0f, 0.0f, 1.0f ),
													color::WHITE,
													vec2f( topLeftTexCoord.x, topLeftTexCoord.y ) ) );


					//consolePrintf( "TexCoords: %s", vertices[ vertices.size() - 1 ].texCoords.toString().c_str() );

					indices.push_back( 4*i + 3 );
					indices.push_back( 4*i + 2 );
					indices.push_back( 4*i );

					indices.push_back( 4*i + 1 );
					indices.push_back( 4*i );
					indices.push_back( 4*i + 2 );

				}


				iter->second.mesh  = new Mesh( vertices, indices, iter->second.tiles[0].tileSet->GetMaterial() );
			}

		}
	}

	void TiledMap::ProcessObjectGroups( XMLParser& parser, XMLNode* root )
	{
		for( XMLNode* objectGroup = root->FirstChildElement( "objectgroup" ); objectGroup != nullptr; objectGroup = objectGroup->NextSiblingElement( "objectgroup" ) )
		{
			std::string name = parser.getXMLAttributeAsString( objectGroup, "name", "" );
			if( name == "spawns" )
			{
				LoadSpawns( parser, objectGroup );
			}
			else if( name == "boundaries" )
			{
				LoadBoundaries( parser, objectGroup );
			}
		}
	}

	void TiledMap::LoadSpawns( XMLParser& parser, XMLNode* spawnObjGroup )
	{
		XMLNode* objectNode = spawnObjGroup->FirstChildElement( "object" );
		for( ; objectNode != nullptr; objectNode = objectNode->NextSiblingElement( "object" ) )
		{
			std::string name = parser.getXMLAttributeAsString( objectNode, "name", "" );
			int x = parser.getXMLAttributeAsInt( objectNode, "x", 0 );
			int y = parser.getXMLAttributeAsInt( objectNode, "y", 0 );

			if( name == "playerspawn" )
			{
				int width = parser.getXMLAttributeAsInt( objectNode, "width", 0 );
				int height = parser.getXMLAttributeAsInt( objectNode, "height", 0 );
				m_playerSpawn.x = x + width * 0.5f;
				m_playerSpawn.y = y + height * 0.5f;
				m_playerSize.x = width * 2.0f;
				m_playerSize.y = height * 2.0f;
			}
			else if( name == "checkpoint" )
			{
				int width = parser.getXMLAttributeAsInt( objectNode, "width", 0 );
				int height = parser.getXMLAttributeAsInt( objectNode, "height", 0 );

				m_checkpoints.push( Checkpoint( vec3f( (float)x, (float)y, 0.0f ), vec2f( (float)width, (float)height ) ) );
				consolePrintf( "Generating checkpoint");
				m_checkpoints.front().mesh = GenerateObjBoxDebugMesh( m_checkpoints.front().box );
			}
		}
	}

	void TiledMap::LoadBoundaries( XMLParser& parser, XMLNode* boundariesGroup )
	{
		XMLNode* objNode = boundariesGroup->FirstChildElement( "object" );
		for( ; objNode != nullptr; objNode = objNode->NextSiblingElement( "object" ) )
		{
			std::string name = parser.getXMLAttributeAsString( objNode, "name", "" );
			int x = parser.getXMLAttributeAsInt( objNode, "x", 0 );
			int y = parser.getXMLAttributeAsInt( objNode, "y", 0 );
			int width = parser.getXMLAttributeAsInt( objNode, "width", 0 );
			int height = parser.getXMLAttributeAsInt( objNode, "height", 0 );

			aabb2f boundBox( vec2f( (float)x, (float)y ), width, height );

			TiledObj& obj = m_objects[ name ];
			obj.boundingBox = boundBox;
			consolePrintf( "Loading bounds: %s", name.c_str() );
			obj.mesh = GenerateObjBoxDebugMesh( boundBox );
		}
	}

	Mesh* GenerateObjBoxDebugMesh( const aabb2f& box )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		Color4f color = color::GREEN;

		float hWidth = box.getWidth() * 0.5f;
		float hHeight = box.getHeight() * 0.5f;
		vec3f pos = box.getCenter();

		consolePrintf( "AABB2 mesh: %s", box.toString().c_str() );

		vertices.push_back( Mesh::Vertex( vec3f( -hWidth, -hHeight ) + pos,
									vec3f(),
									color,
									vec2f( 0.0f, 1.0f ) ) );


		vertices.push_back( Mesh::Vertex( vec3f( hWidth, -hHeight ) + pos,
									vec3f(),
									color,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( hWidth, hHeight ) + pos,
									vec3f(),
									color,
									vec2f( 1.0f, 0.0f ) ) );


		vertices.push_back( Mesh::Vertex( vec3f( -hWidth, hHeight ) + pos,
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );


		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );

		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, "MusheesRunMat" );
	}
}
