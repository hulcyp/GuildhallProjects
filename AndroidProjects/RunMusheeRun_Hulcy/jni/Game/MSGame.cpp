#include "MSGame.h"
#include "../MonkyRenderer/SpriteAnimation.h"
#include "../MonkyRenderer/SpriteActor.h"
#include "../MonkyRenderer/MeshFactory.h"

namespace Monky
{
	MSGame::MSGame( double deltaTime, int width, int height, float fov )
		:	GameApp( deltaTime, width, height, fov )
	{}

	MSGame::~MSGame()
	{

	}

	void MSGame::initialize()
	{
		GameApp::initialize();

		Mesh* platformMesh = MeshFactory::generate2DOrthoRectangle( 100.f, 100.f, "GreenMushroom" );
		Actor* platform = spawn( "zgreenPlatform", platformMesh );
		platform->setPosition( vec3f( m_screenWidth / 2, m_screenHeight / 2, -1.0f ) );


		m_spinningCube = new SpriteActor("Mushee", 100.0f, "MusheesRunMat" );
		SpriteAnimation* animation = new SpriteAnimation( "animations/MusheesYellow.xml" );
		m_spinningCube->AddAnimation( "Run", animation );
		m_spinningCube->PlayAnimation( "Run" );
		spawn( m_spinningCube );
		m_spinningCube->setPosition( vec3f( m_screenWidth / 2, m_screenHeight / 2, 0.0f ) );



	}

	bool MSGame::onFingerDown( int fingerID, float x, float y )
	{
		consolePrintf( "Finger down(%f,%f):%d", x, y, fingerID );
		if( fingerID == 0 )
		{
			m_lastPosDown.x = x;
			m_lastPosDown.y = y;
		}
		return true;
	}

	bool MSGame::onFingerUp( int fingerID, float x, float y )
	{
		if( fingerID == 0 )
		{
			if( m_lastPosDown.y > y )
			{
				m_spinningCube->setPosition( m_spinningCube->getPosition() + vec3f( 0.0f, -20.0f ) );
			}
			else if( m_lastPosDown.y < y )
			{
				m_spinningCube->setPosition( m_spinningCube->getPosition() + vec3f( 0.0f, 20.0f ) );
			}
		}
		return true;
	}

	void MSGame::updateSimulation()
	{
		GameApp::updateSimulation();
		//consolePrintf( "Updating Game" );
	}



}
