#include "Spaceship.h"
#include <Mesh.h>
#include "Timer.h"


namespace Monky
{
	Spaceship::Spaceship( const std::string& spaceshipName, short teamNumber, int screenWidth, int screenHeight, bool isServerSide )
		:	SpaceGameActor( spaceshipName, teamNumber, screenWidth, screenHeight )
		,	m_spaceShipName( spaceshipName )
	{
		m_actorType = SPACE_SHIP_ACTOR_TYPE;
		m_radius = 20.0f;
		m_drag = 15.0f;
		if( !isServerSide )
			createMesh( getTeamColor( teamNumber ) );
		
		m_weaponTimer = new Timer( 0.5f );
		m_weaponTimer->start();

		m_actorTypesCanCollideWith = BULLET_ACTOR_TYPE;

		m_fontParams.set( "fontName", std::string( "mainFont_72" ) );
		m_fontParams.set( "fontHeight", 12 );
		m_fontParams.set( "color", getTeamColor( teamNumber ) );
		m_fontParams.set( "posRel", std::string( "BOTLEFT" ) );
		m_fontParams.set( "text", spaceshipName );
	}	
	//----------------------------------------------------------------------
	Spaceship::~Spaceship()
	{
		NamedProperties params;
		params.set( "spaceship", this );
		fireEvent( "SpaceshipDied", params );
	}
	//----------------------------------------------------------------------
	void Spaceship::render()
	{
		mat4f translation( mat4f::IDENTITY );
		translation.translate( getPosition() );
		Object::render();
		m_fontParams.set( "modelMatrix", translation );
		fireEvent( "renderText", m_fontParams );
	}
	//----------------------------------------------------------------------
	void Spaceship::fireBullet()
	{
		if( m_weaponTimer->isTimeUp() )
		{
			NamedProperties params;
			params.set( "pos", getPosition() );
			params.set( "rotation", m_rotation );
			params.set( "teamNumber", m_teamNumber );
			fireEvent( "SpawnBullet", params );
			m_weaponTimer->start();
		}
	}
	//----------------------------------------------------------------------
	void Spaceship::assignTeamNumber( short teamNumber )
	{
		m_teamNumber = teamNumber;
	}
	//----------------------------------------------------------------------
	void Spaceship::createMesh( const Color4f& color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;
		
		vertices.push_back(	Mesh::Vertex(	vec3f( 0.0f, 20.0f ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f() ) );
		vertices.push_back(	Mesh::Vertex(	vec3f( 15.0f, -20.0f ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f() ) );
		vertices.push_back(	Mesh::Vertex(	vec3f( 0.0f, -10.0f ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f() ) );
		vertices.push_back(	Mesh::Vertex(	vec3f( -15.0f, -20.0f ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f() ) );
		
		indices.push_back( 0 );
		indices.push_back( 1 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		setMesh( new Mesh( vertices, indices, "SimpleColorMaterial", GL_LINE_LOOP ) );
	}
	//----------------------------------------------------------------------
	Color4f getTeamColor( short teamNumber )
	{
		static bool isInitialized = false;
		static Color4f teamColors[4];

		if( !isInitialized )
		{
			teamColors[0] = color::GREEN;
			teamColors[1] = color::RED;
			teamColors[2] = color::BLUE;
			teamColors[3] = color::YELLOW;
		}

		assertion( teamNumber >= 0 && teamNumber < 4, "Team number out of range: %d", teamNumber );
		return teamColors[ teamNumber ];
	}
}