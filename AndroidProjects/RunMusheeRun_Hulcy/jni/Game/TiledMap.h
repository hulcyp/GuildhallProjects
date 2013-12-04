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

	struct Checkpoint
	{
		Checkpoint( const vec3f& pos, const vec2f& size )
			:	box( pos.xy() - size * 0.5f, size )
		{
		}
		aabb2f box;
	};

	class TiledMap
	{
	public:
		TiledMap( const std::string& mapFile );

		void RenderMap();

		void ReloadLayers();

		const vec3f& GetPlayerSpawn() { return m_playerSpawn; }
		const vec2f& GetPlayerSize() { return m_playerSize; }

		bool DidPlayerCollideWithCheckpoint( const aabb2f& playerBox, vec2f& checkpointPos );

	private:
		struct MapVertex
		{
			MapVertex( const vec3f& p, const vec2f& tc, const Color4f& c = color::WHITE )
				:	pos( p )
				,	texCoords( tc )
				,	color( c )
			{}
			vec3f pos;
			vec2f texCoords;
			Color4f color;
		};

		void LoadTileSets( XMLParser& parser, XMLNode* root );
		void LoadTileLayers( XMLParser& parser, XMLNode* root );
		void ConstructTileLayerMeshes();
		void ProcessObjectGroups( XMLParser& parser, XMLNode* root );
		void LoadSpawns( XMLParser& parser, XMLNode* spawnObjGroup );


		TileSet* GetTileSetGIDIsIn( int gid );


		std::vector< TileLayer > m_layers;

		std::vector< TileSet > m_tileSets;
		const std::string m_mapFile;
		int m_width;
		int m_height;
		int m_tileWidth;
		int m_tileHeight;

		std::queue< Checkpoint > m_checkpoints;
		vec3f m_playerSpawn;
		vec2f m_playerSize;


	};
}
