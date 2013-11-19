#include "App.h"
#include "Spaceship.h"
#include "LocalPlayerController.h"
#include "Bullet.h"
#include "CollisionManager.h"

namespace Monky
{
	App::App(  double deltaTime, int width, int height, float fov, const char* windowTitle )
		:	GameApp( deltaTime, width, height, fov, windowTitle )
	{
		m_collisionManager = new CollisionManager();

		m_ship = new Spaceship( "Enterprise", 0, width, height );
		m_ship->setPosition( vec3f( 50.f, 50.f ) );
		spawn( m_ship );
		
		for( int i = 0; i < 3; ++i )
		{
			Spaceship* spaceship = new Spaceship( "Prometheus", i + 1, width, height );
			spawn( spaceship );
			spaceship->setPosition( vec3f( 50.0f * i, 50.f * i ) );
		}
		m_localPlayer = new LocalPlayerController( 0 );
		m_localPlayer->assignShip( m_ship );

		registerForEvent( "SpawnBullet", *this, &App::spawnBulletEvt );
	}
	//----------------------------------------------------------------------
	App::~App()
	{}
	//----------------------------------------------------------------------
	bool App::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		used = GameApp::onKeyDown( keyCode, x, y );

		return used;
	}
	//----------------------------------------------------------------------
	void App::spawnBulletEvt( NamedProperties& params )
	{
		vec3f pos;
		float rotation;
		int teamNumber = 0;
		if( params.get( "pos", pos ) == NamedProperties::SUCCESS )
		{
			if( params.get( "rotation", rotation ) == NamedProperties::SUCCESS )
			{
				params.get( "teamNumber", teamNumber );
				Bullet* bullet = new Bullet( vec2f( pos.x, pos.y ), rotation, teamNumber, m_screenWidth, m_screenHeight );
				spawn( bullet );
			}
		}
	}
	//----------------------------------------------------------------------
	void App::updateSimulation()
	{
		m_localPlayer->update( DELTA_TIME );
		GameApp::updateSimulation();
		m_collisionManager->udpate( DELTA_TIME );
	}
	//----------------------------------------------------------------------
	void App::updateDisplay()
	{
		GameApp::updateDisplay();
		GameApp::renderHUD();
		GameApp::renderDebugHUD();
		GameApp::renderConsole();
	}
	//----------------------------------------------------------------------
	void App::renderDebugHUD()
	{
		GameApp::renderDebugHUD();
	}
}