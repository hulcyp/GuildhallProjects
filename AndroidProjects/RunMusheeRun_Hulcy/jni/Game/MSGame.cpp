#include "MSGame.h"
#include "../MonkyRenderer/SpriteAnimation.h"
#include "../MonkyRenderer/SpriteActor.h"
#include "../MonkyRenderer/MeshFactory.h"
#include "../MonkyRenderer/Renderer.h"

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

		//Mesh* platformMesh = MeshFactory::generate2DOrthoRectangle( 100.f, 100.f, "GreenMushroom" );
		//Actor* platform = spawn( "zgreenPlatform", platformMesh );
		//platform->setPosition( vec3f( m_screenWidth / 2, m_screenHeight / 2, -1.0f ) );


		m_tiledMap = new TiledMap("levels/level01.tmx");

		m_spinningCube = new SpriteActor("Mushee", m_tiledMap->GetPlayerSize().x, "MusheesRunMat" );
		SpriteAnimation* animation = new SpriteAnimation( "animations/MusheesYellow.xml" );
		m_spinningCube->AddAnimation( "Run", animation );
		m_spinningCube->PlayAnimation( "Run" );
		spawn( m_spinningCube );
		m_spinningCube->setPosition( m_tiledMap->GetPlayerSpawn() );



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
			if( m_lastPosDown.y < y )
			{
				m_spinningCube->setPosition( m_spinningCube->getPosition() + vec3f( 0.0f, -20.0f ) );
			}
			else if( m_lastPosDown.y > y )
			{
				m_spinningCube->setPosition( m_spinningCube->getPosition() + vec3f( 0.0f, 20.0f ) );
			}
		}
		return true;
	}

	void MSGame::reloadRenderingAssets()
	{
		GameApp::reloadRenderingAssets();
		m_tiledMap->ReloadLayers();
	}

	void MSGame::updateSimulation()
	{
		GameApp::updateSimulation();
		//consolePrintf( "Updating Game" );
	}

	void MSGame::updateDisplay()
	{
		PROFILE_SECTION( "GameApp" );
		glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		m_renderer->pushCamera( m_camera );
		if( m_camera )
		{
			m_camera->update( DELTA_TIME );
		}
		//m_renderer->setWireFrame( m_wireFrame );
		m_tiledMap->RenderMap();
		m_actorManager.renderActors();
		renderDebugHUD();
		//m_GDOManager->renderGDOs( m_renderer, m_frameClock.getElapsedSecondsFloat() );
		m_renderer->popCamera();
		ProfileSystem::getInstance()->clearFrameData();
	}



}
