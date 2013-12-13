#include "MSMap.h"
#include "MonkyRenderer/Mesh.h"
#include "CoreEngine/XMLParser.h"
#include "MathLib/RandNumGen.h"
#include "CoreEngine/TimeUtils.h"
#include "MathLib/MathFuncs.h"
#include "MonkyRenderer/MeshFactory.h"

#define MAX_SPAWN_RATE 5.0f
#define MIN_SPAWN_RATE 1.0f
#define STARTING_NUMBER_LAYERS 3

namespace Monky
{



	MSMap::MSMap( const vec2f& mapOffsetFromPlayerPos, float floorTileX, float floorTileY, float mapWidth, float mapHeight, const std::string& floorTileMaterial )
		:	m_mapSize( mapWidth, floorTileY )
		,	m_screenSize( mapWidth, mapHeight )
		,	m_lastLayerSpawnedOn( 0 )
		,	m_rateToDecreaseSpawnRate( 0.1f )
		,	m_currentSpawnRate( MAX_SPAWN_RATE )
		,	m_spawnTimer( MAX_SPAWN_RATE )
		,	m_verticalOffsetBetweenLayers( 50.0f )
		,	m_playerSpawn( vec2f( mapWidth * 0.5f, mapHeight * 0.5f ) - vec2f(0.0f, 50.0f ) )
		,	m_mapOffsetFromPlayerPos( mapOffsetFromPlayerPos )

	{
		m_currentObstaclesOnMap.resize( STARTING_NUMBER_LAYERS );
		GenerateMapMesh( floorTileX, floorTileY, mapWidth, floorTileMaterial );
		m_mapYPos = ( m_screenSize.y - m_mapSize.y ) * 0.5f;
	}

	MSMap::~MSMap()
	{
		VECTOR_CLEANUP( m_obstacleBlueprints );
		for( int c = 0; c < m_currentObstaclesOnMap.size(); ++c )
			VECTOR_CLEANUP( m_currentObstaclesOnMap[c] );
		SAFE_DELETE( m_platformMesh );
	}

	void MSMap::LoadObstacles( const std::string& fileName )
	{
		XMLParser parser( fileName.c_str() );
		XMLDocument& doc = parser.getDocument();

		if( !doc.Error() )
		{
			XMLNode* root = doc.FirstChildElement( "Obstacles" );
			XMLNode* obstacle = root->FirstChildElement( "Obstacle" );
			for( ; obstacle != nullptr; obstacle = obstacle->NextSiblingElement( "Obstacle" ) )
			{
				std::string material = parser.getXMLAttributeAsString( obstacle, "material", "" );
				vec2f dimensions = parser.getXMLAttributeAsVec2( obstacle, "dimensions", vec2f() );
				if( material != "" )
				{
					AddRandomObstacle( new Obstacle( material, aabb2f( vec2f(), dimensions.x, dimensions.y ) ) );
				}
			}
		}
	}

	void MSMap::AddRandomObstacle( Obstacle* obstacle )
	{
		m_obstacleBlueprints.push_back( obstacle );
		consolePrintf( "Spawning obstacle blueprint" );
	}

	void MSMap::Update( Camera* camera, const aabb2f& playerBox, float deltaTime )
	{
		if( m_spawnTimer > 0.0f )
		{
			m_spawnTimer -= deltaTime;
		}
		else
		{
			SpawnNewObstacle( camera, playerBox );
			m_spawnTimer = m_currentSpawnRate;
			m_currentSpawnRate -= m_rateToDecreaseSpawnRate;
			m_currentSpawnRate = MathFuncs<float>::clamp( m_currentSpawnRate, MIN_SPAWN_RATE, MAX_SPAWN_RATE );
		}

		CullOldObstacles( camera, playerBox );

		m_currentLocation = playerBox.getCenter() - m_mapOffsetFromPlayerPos;
	}

	void MSMap::Render()
	{
		NamedProperties params;
		matStackf matStack;
		matStack.loadIdentity();
		m_currentLocation.y = m_mapYPos;
		matStack.translate( m_currentLocation );
		params.set( "modelMatrix", matStack.top() );
		params.set( "mesh", m_platformMesh );
		fireEvent( "renderMesh", params );
	}

	void MSMap::RenderLayer( int layer )
	{
		NamedProperties params;
		matStackf matStack;
		matStack.loadIdentity();

		if( 0 <= layer && layer < m_currentObstaclesOnMap.size() )
		{
			for( int i = 0; i < m_currentObstaclesOnMap[layer].size(); ++i )
			{
				matStack.loadIdentity();
				matStack.translate( m_currentObstaclesOnMap[layer][i]->boundingBox.getCenter() );
				params.set( "modelMatrix", matStack.top() );
				params.set( "mesh", m_currentObstaclesOnMap[layer][i]->mesh );
				fireEvent( "renderMesh", params );
			}
		}
	}


	bool MSMap::DidPlayerCollideWithObstacle( const aabb2f& playerBox, int currentLayer ) const
	{
		bool didCollide = false;
		vec2f intersectionDistance;
		if( 0 <= currentLayer && currentLayer < m_currentObstaclesOnMap.size() )
		{
			for( int i = 0; i < m_currentObstaclesOnMap[currentLayer].size(); ++i )
			{
				if( m_currentObstaclesOnMap[currentLayer][i]->boundingBox.intersects( playerBox, intersectionDistance ) )
				{
					didCollide = true;
					break;
				}
			}
		}

		return didCollide;
	}

	void MSMap::ReloadRenderingAssets()
	{
		m_platformMesh->reloadBuffers();
	}

	void MSMap::ResetMap()
	{
		for( int layer = 0; layer < m_currentObstaclesOnMap.size(); ++layer )
		{
			VECTOR_CLEANUP( m_currentObstaclesOnMap[layer] );
		}
		m_currentObstaclesOnMap.resize( STARTING_NUMBER_LAYERS );
		m_currentSpawnRate  = MAX_SPAWN_RATE;
		m_spawnTimer = MAX_SPAWN_RATE;
	}

	float MSMap::GetMinYPosForLayer( int layer )
	{
		return ( layer - (int)m_currentObstaclesOnMap.size() / 2 ) * m_verticalOffsetBetweenLayers + m_playerSpawn.y;
	}

	void MSMap::GenerateMapMesh( float floorTileX, float floorTileY, float mapWidth, const std::string& floorTileMaterial )
	{
		vec3f currentTopLeftCorner;

		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;
		int i = 0;

		while( currentTopLeftCorner.x < mapWidth )
		{
			vertices.push_back( Mesh::Vertex( vec3f( currentTopLeftCorner.x, currentTopLeftCorner.y + floorTileY, currentTopLeftCorner.z ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color::WHITE,
											vec2f( 0.0f, 1.0f ) ) );


			vertices.push_back( Mesh::Vertex( vec3f( currentTopLeftCorner.x + floorTileX, currentTopLeftCorner.y + floorTileY, currentTopLeftCorner.z ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color::WHITE,
											vec2f( 1.0f, 1.0f ) ) );

			vertices.push_back( Mesh::Vertex( vec3f( currentTopLeftCorner.x + floorTileX, currentTopLeftCorner.y, currentTopLeftCorner.z ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color::WHITE,
											vec2f( 1.0f , 0.0f ) ) );


			vertices.push_back( Mesh::Vertex( vec3f( currentTopLeftCorner.x, currentTopLeftCorner.y, currentTopLeftCorner.z ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color::WHITE,
											vec2f() ) );

			indices.push_back( 4*i + 3 );
			indices.push_back( 4*i + 2 );
			indices.push_back( 4*i );

			indices.push_back( 4*i + 1 );
			indices.push_back( 4*i );
			indices.push_back( 4*i + 2 );

			currentTopLeftCorner.x += floorTileX;
			++i;
		}

		m_platformMesh = new Mesh( vertices, indices, floorTileMaterial );
	}

	void MSMap::SpawnNewObstacle( Camera* camera, const aabb2f& playerBox )
	{
		RandNumGen::seed( (unsigned int)TimeUtils::GetAbsoluteTime( TimeUtils::MILLI ) );
		int obstacleIdx = RandNumGen::randInRangeInt( 0, m_obstacleBlueprints.size() );
		Obstacle* blueprint = m_obstacleBlueprints[ obstacleIdx ];
		int layerToSpawnOn = RandNumGen::randInRangeInt( 0, (int)m_currentObstaclesOnMap.size() );


		if( blueprint != nullptr )
		{
			consolePrintf( "Spawning obstacle" );
			vec3f spawnPos = m_playerSpawn;
			spawnPos.x = playerBox.getCenter().x + m_mapSize.x -  m_mapOffsetFromPlayerPos.x;
			spawnPos.y = ( layerToSpawnOn - (int)m_currentObstaclesOnMap.size() / 2 ) * m_verticalOffsetBetweenLayers + spawnPos.y;
			Obstacle* newObstacle = new Obstacle( blueprint->material, blueprint->boundingBox );
			newObstacle->boundingBox.setPositionFromCenter( spawnPos.xy() );
			newObstacle->mesh = GenerateMeshFromAABB2( newObstacle->boundingBox, newObstacle->material );
			m_currentObstaclesOnMap[layerToSpawnOn].push_back( newObstacle );
		}
	}

	void MSMap::CullOldObstacles( Camera* camera, const aabb2f& playerBox )
	{
		float playerXPos = playerBox.getCenter().x;
		float thresholdX = playerXPos - m_mapOffsetFromPlayerPos.x - 10.0f;

		for( int i = 0; i < m_currentObstaclesOnMap.size(); ++i )
		{
			m_currentObstaclesOnMap[i].erase(
					std::remove_if( m_currentObstaclesOnMap[i].begin(), m_currentObstaclesOnMap[i].end(),
						[&]( Obstacle* obstacle )
						{
							return obstacle->boundingBox.getCenter().x < thresholdX;
						} )
			, m_currentObstaclesOnMap[i].end() );
		}

	}

	Mesh* GenerateMeshFromAABB2( const aabb2f& box, const std::string& material )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		Color4f color = color::WHITE;

		float hWidth = box.getWidth() * 0.5f;
		float hHeight = box.getHeight() * 0.5f;

		vertices.push_back( Mesh::Vertex( vec3f( -hWidth, -hHeight ),
									vec3f(),
									color,
									vec2f( 0.0f, 0.0f ) ) );


		vertices.push_back( Mesh::Vertex( vec3f( hWidth, -hHeight ),
									vec3f(),
									color,
									vec2f( 1.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( hWidth, hHeight ),
									vec3f(),
									color,
									vec2f( 1.0f, 1.0f ) ) );


		vertices.push_back( Mesh::Vertex( vec3f( -hWidth, hHeight ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );


		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );

		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, material );
	}


}
