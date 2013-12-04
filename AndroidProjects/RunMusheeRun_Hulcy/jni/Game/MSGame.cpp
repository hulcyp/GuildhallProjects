#include "MSGame.h"
#include "../MonkyRenderer/SpriteAnimation.h"
#include "../MonkyRenderer/SpriteActor.h"
#include "../MonkyRenderer/MeshFactory.h"
#include "../MonkyRenderer/Renderer.h"

namespace Monky
{
	MSGame::MSGame( double deltaTime, int width, int height, float fov )
		:	GameApp( deltaTime, width, height, fov )
		,	m_currentHorizontalImpulseToApply( 200.0f )
		, 	m_verticalImpulseToApply( 50.0f )
	{}

	MSGame::~MSGame()
	{}

	void MSGame::initialize()
	{
		GameApp::initialize();


		m_tiledMap = new TiledMap("levels/level01.tmx");

		m_theMushee = new SpriteActor("Mushee", m_tiledMap->GetPlayerSize().x, "MusheesRunMat", 10.0f );
		SpriteAnimation* animation = new SpriteAnimation( "animations/MusheesYellow.xml" );
		m_theMushee->AddAnimation( "Run", animation );
		m_theMushee->PlayAnimation( "Run" );
		spawn( m_theMushee );
		m_theMushee->setPosition( m_tiledMap->GetPlayerSpawn() );

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
				m_theMushee->setPosition( m_theMushee->getPosition() + vec3f( 0.0f, -m_verticalImpulseToApply ) );
			}
			else if( m_lastPosDown.y > y )
			{
				m_theMushee->setPosition( m_theMushee->getPosition() + vec3f( 0.0f, m_verticalImpulseToApply ) );
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

		m_theMushee->ApplyImpulse( vec2f( m_currentHorizontalImpulseToApply ) );

		vec3f newCameraPos;
		newCameraPos.x = m_theMushee->getPosition().x - m_screenWidth * 0.5f;

		m_camera->setPosition( newCameraPos );

		vec2f newCheckpointLoc;
		if( m_tiledMap->DidPlayerCollideWithCheckpoint( m_theMushee->GetBoundingBox(), newCheckpointLoc ) )
		{
			m_currentCheckpoint =  newCheckpointLoc;
		}
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
		m_tiledMap->RenderMap();
		m_actorManager.renderActors();
		renderDebugHUD();
		//m_GDOManager->renderGDOs( m_renderer, m_frameClock.getElapsedSecondsFloat() );
		m_renderer->popCamera();
		ProfileSystem::getInstance()->clearFrameData();
	}



}
