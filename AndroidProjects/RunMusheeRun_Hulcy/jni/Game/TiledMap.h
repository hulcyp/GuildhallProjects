#pragma once
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "TileSet.h"
#include "CoreEngine/XMLParser.h"

namespace Monky
{
	class Mesh;

	struct Tile
	{
		TileSet* tileSet;
		int gid;
		vec3f pos;
	};

	struct TileLayerTileGroup
	{
		Mesh* mesh;
		std::vector< Tile > tiles;
	};

	struct TileLayer
	{
		std::string name;
		int width;
		int height;
		std::map< std::string, TileLayerTileGroup > tileGroups;
	};

	class TiledMap
	{
	public:
		TiledMap( const std::string& mapFile );

		void RenderMap();

	private:
		struct MapVertex
		{
			MapVertex( const vec3f& pos, const vec2f& texCoords, const Color4f& color = color::WHITE )
				:	pos( pos )
				,	texCoords( texCoords )
				,	color( color )
			{}
			vec3f pos;
			Color4f color;
			vec2f texCoords;
		};

		void LoadTileSets( XMLParser& parser, XMLNode* root );
		void LoadTileLayers( XMLParser& parser, XMLNode* root );
		void ConstructTileLayerMeshes();

		vec3f GetLocationFromIndex( int i );

		TileSet* GetTileSetGIDIsIn( int gid );


		std::vector< TileLayer > m_layers;

		std::vector< TileSet > m_tileSets;
		const std::string m_mapFile;
		int m_width;
		int m_height;
		int m_tileWidth;
		int m_tileHeight;
	};
}
