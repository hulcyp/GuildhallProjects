#pragma once
#include "CoreEngine/CommonCoreEngineIncludes.h"

namespace Monky
{
	class Camera;
	class Mesh;

	class MSMap
	{
	public:


		struct Obstacle
		{
			Obstacle( const std::string& material, const aabb2f& boundingBox )
				:	material( material )
				,	boundingBox( boundingBox )
			{}
			std::string material;
			Mesh* mesh;
			aabb2f boundingBox;
		};

		MSMap( const vec2f& mapOffsetFromPlayerPos, float floorTileX, float floorTileY, float mapWidth, float mapHeight, const std::string& floorTileMaterial );
		~MSMap();

		void LoadObstacles( const std::string& fileName );
		void AddRandomObstacle( Obstacle* obstacle );

		void Update( Camera* camera, const aabb2f& playerBox, float deltaTime );
		void Render();

		void RenderLayer( int layer );

		bool DidPlayerCollideWithObstacle( const aabb2f& playerBox, int currentLayer ) const;

		void ReloadRenderingAssets();

		void ResetMap();

		const vec3f GetPlayerSpawn() const { return m_playerSpawn; }
		float GetOffsetBetweenLayers() const { return m_verticalOffsetBetweenLayers; }

		int GetTopLayer() const { return 0; }
		int GetBottomLayer() const { return (int)m_currentObstaclesOnMap.size() - 1; }
		int GetMiddleLayer() const { return (int)m_currentObstaclesOnMap.size() / 2; }

		float GetMinYPosForLayer( int layer );

	protected:
		void GenerateMapMesh( float floorTileX, float floorTileY, float mapWidth, const std::string& floorTileMaterial );
		void SpawnNewObstacle( Camera* camera, const aabb2f& playerBox );
		void CullOldObstacles( Camera* camera, const aabb2f& playerBox );


	private:
		std::vector< std::vector< Obstacle* > > m_currentObstaclesOnMap;
		std::vector< Obstacle* > m_obstacleBlueprints;
		Mesh* m_platformMesh;
		vec2f m_screenSize;
		vec2f m_mapSize;
		float m_verticalOffsetBetweenLayers;
		float m_mapYPos;
		int m_lastLayerSpawnedOn;
		float m_rateToDecreaseSpawnRate;
		float m_currentSpawnRate;
		float m_spawnTimer;
		const vec3f m_playerSpawn;
		vec3f m_currentLocation;
		vec2f m_mapOffsetFromPlayerPos;
	};

	Mesh* GenerateMeshFromAABB2( const aabb2f& box, const std::string& material );

}
