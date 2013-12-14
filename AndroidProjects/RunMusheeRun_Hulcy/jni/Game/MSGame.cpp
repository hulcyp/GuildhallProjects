#include "MSGame.h"
#include "../MonkyRenderer/SpriteAnimation.h"
#include "SpriteActor.h"
#include "../MonkyRenderer/MeshFactory.h"
#include "../MonkyRenderer/Renderer.h"
#include "MSMap.h"
#include "Button.h"


#define BUTTON_SIZE 200.0f
namespace Monky
{
	MSGame::MSGame( double deltaTime, int width, int height, float fov )
		:	GameApp( deltaTime, width, height, fov )
		,	m_currentHorizontalImpulseToApply( 500.0f )
		, 	m_verticalImpulseToApply( 0.0f )
		,	m_currentPlayerLayer( 1 )		//At start 1 is the middle layer cause the game will start with 3 layers
		,	m_respawnTimer( 5.0f )
		,	m_respawnTime( 5.0f )
		,	m_isPaused( false )
		,	m_jumpImpulse( 0.0f, -3500.0f )
		,	m_gravity( 0.0f, 100.0f )
		,	m_cameraOffset( m_screenWidth * 0.2f )
		,	m_playerScore( 0 )
		,	m_increaseScoreRate( 0.1f )
	{}

	MSGame::~MSGame()
	{}

	void MSGame::initialize()
	{
		GameApp::initialize();

		m_hudCamera = new Camera( "hudCamera", vec3f(), Camera::PH_ORTHOGRAPHIC,
				mat4f::ortho( 0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight, -1.0f, 1.0f ) );

		m_randomMap = new MSMap( vec2f( m_screenWidth * 0.2f, 0.0f), 152, 242, m_screenWidth, m_screenHeight, "MushroomPlatform" );
		m_verticalImpulseToApply = m_randomMap->GetOffsetBetweenLayers();
		m_randomMap->LoadObstacles( "xml/Obstacles.xml" );
		SpawnMushee();

		//Down button
		Button* button =
				new Button(
						aabb2f( vec2f( m_screenWidth - BUTTON_SIZE, m_screenHeight - BUTTON_SIZE ),
								BUTTON_SIZE, BUTTON_SIZE),
						"DownArrow" );
		button->SetEventToFire( "MoveMusheeDown" );
		button->SetDownMaterial( "DownArrow_Down" );
		m_buttons.push_back( button );
		//-----------------------------------------------------------------

		//Up button
		button =
			new Button(
					aabb2f(vec2f( m_screenWidth - BUTTON_SIZE, m_screenHeight - 2.0f * BUTTON_SIZE ),
							BUTTON_SIZE, BUTTON_SIZE),
					"UpArrow" );
		button->SetEventToFire( "MoveMusheeUp" );
		button->SetDownMaterial( "UpArrow_Down" );
		m_buttons.push_back( button );

		//Jump button
		button =
			new Button(
					aabb2f(vec2f( 0.0f, m_screenHeight - BUTTON_SIZE ),
							BUTTON_SIZE, BUTTON_SIZE),
					"UpArrow" );
		button->SetEventToFire( "MusheeJump" );
		button->SetDownMaterial( "UpArrow_Down" );
		m_buttons.push_back( button );

		//-----------------------------------------------------------------

		//Pause Button
		button =
			new Button(
					aabb2f(vec2f( m_screenWidth*0.5f - BUTTON_SIZE * 0.5f, m_screenHeight - BUTTON_SIZE ),
							BUTTON_SIZE, BUTTON_SIZE),
					"PauseButton_Up" );
		button->SetEventToFire( "PauseGame" );
		button->SetDownMaterial( "PauseButton_Down" );
		m_buttons.push_back( button );

		m_bgMesh = GenerateMeshFromAABB2( aabb2f( vec2f(), m_screenWidth, m_screenHeight ), "Mushees_Background" );
		m_bgParams.set( "mesh", m_bgMesh );

		//-----------------------------------------------------------------

		registerForEvent( "MoveMusheeUp", *this, &MSGame::MoveMusheeUp );
		registerForEvent( "MoveMusheeDown", *this, &MSGame::MoveMusheeDown );
		registerForEvent( "PauseGame", *this, &MSGame::PauseGame );
		registerForEvent( "MusheeJump", *this, &MSGame::MusheeJump );

		m_gameMessageTextParams.set( "fontName", std::string( "mainFont_72" ) );
		m_gameMessageTextParams.set( "fontHeight", 96.0f );
		m_gameMessageTextParams.set( "color", color::GREEN );
		m_gameMessageTextParams.set( "posRel", std::string( "CENTER" ) );
		m_gameMessageTextParams.set( "camera", m_debugCamera );
		m_gameMessageTextParams.set( "text", std::string( "Paused" ) );
		mat4f modelMatrix( mat4f::IDENTITY );
		modelMatrix.translate( vec3f( m_screenWidth * 0.5f, m_screenHeight * 0.5f, 0.0f ) );
		m_gameMessageTextParams.set( "modelMatrix", modelMatrix );


		m_scoreTextParams.set( "fontName", std::string( "mainFont_72" ) );
		m_scoreTextParams.set( "fontHeight", 96.0f );
		m_scoreTextParams.set( "color", color::GREEN );
		m_scoreTextParams.set( "posRel", std::string( "TOPRIGHT" ) );
		m_scoreTextParams.set( "camera", m_debugCamera );
		modelMatrix.loadIdentity();
		modelMatrix.translate( vec3f( m_screenWidth, m_screenHeight - 35.0f ) );
		m_scoreTextParams.set( "modelMatrix", modelMatrix );
	}

	bool MSGame::onFingerDown( int fingerID, float x, float y )
	{
		if( fingerID == 0 )
		{
			m_lastPosDown.x = x;
			m_lastPosDown.y = y;
		}
		for( int i = 0; i < m_buttons.size(); ++i )
		{
			m_buttons[i]->FingerDown( vec2f( x, y ) );
		}
		return true;
	}

	bool MSGame::onFingerUp( int fingerID, float x, float y )
	{
		for( int i = 0; i < m_buttons.size(); ++i )
		{
			m_buttons[i]->FingerUp( vec2f( x, y ) );
		}

		return true;
	}

	void MSGame::reloadRenderingAssets()
	{
		GameApp::reloadRenderingAssets();
		m_randomMap->ReloadRenderingAssets();
		for( int i = 0; i < m_buttons.size(); ++i )
		{
			if( m_buttons[i] != nullptr )
			{
				m_buttons[i]->ReloadMesh();
			}
		}
		m_bgMesh->reloadBuffers();
	}


	void MSGame::MoveMusheeUp( NamedProperties& params )
	{
		float minYPosForLayer = m_randomMap->GetMinYPosForLayer( m_currentPlayerLayer );
		if( m_theMushee != nullptr && !m_isPaused && m_theMushee->getPosition().y == minYPosForLayer )
		{
			--m_currentPlayerLayer;
			if( m_currentPlayerLayer < m_randomMap->GetTopLayer() )
			{
				m_currentPlayerLayer = m_randomMap->GetTopLayer();
			}
			else
			{
				m_theMushee->setPosition( m_theMushee->getPosition() + vec3f( 0.0f, -m_verticalImpulseToApply ) );
			}
		}
	}

	void MSGame::MoveMusheeDown( NamedProperties& params )
	{
		float minYPosForLayer = m_randomMap->GetMinYPosForLayer( m_currentPlayerLayer );
		if( m_theMushee != nullptr && !m_isPaused && m_theMushee->getPosition().y == minYPosForLayer )
		{
			++m_currentPlayerLayer;
			if( m_currentPlayerLayer > m_randomMap->GetBottomLayer() )
			{
				m_currentPlayerLayer = m_randomMap->GetBottomLayer();
			}
			else
			{
				m_theMushee->setPosition( m_theMushee->getPosition() + vec3f( 0.0f, m_verticalImpulseToApply ) );
			}
		}
	}

	void MSGame::MusheeJump( NamedProperties& params )
	{
		float minYPosForLayer = m_randomMap->GetMinYPosForLayer( m_currentPlayerLayer );
		if( m_theMushee != nullptr && !m_isPaused && m_theMushee->getPosition().y == minYPosForLayer )
		{
			m_theMushee->ApplyImpulse( m_jumpImpulse );
			consolePrintf( "Jump" );
		}
	}

	void MSGame::PauseGame( NamedProperties& params )
	{
		m_isPaused = !m_isPaused;
		if( m_theMushee != nullptr )
			m_theMushee->SetPauseUpdate( m_isPaused );
	}

	void MSGame::updateSimulation()
	{
		GameApp::updateSimulation();

		if( !m_isPaused )
		{

			if( m_theMushee != nullptr )
			{
				m_theMushee->ApplyGravity( m_gravity );
				m_theMushee->ApplyImpulse( vec2f( m_currentHorizontalImpulseToApply ) );

				vec3f newCameraPos;
				newCameraPos.x = m_theMushee->getPosition().x - m_cameraOffset;

				m_camera->setPosition( newCameraPos );
				m_randomMap->Update( m_camera, m_theMushee->GetBoundingBox(), DELTA_TIME );

				float x = m_theMushee->GetBoundingBox().getCenter().x - m_cameraOffset + m_screenWidth * 0.5f;
				float y = m_screenHeight * 0.5f;
				m_bgPosition = vec2f( x, y );

				if( m_randomMap->DidPlayerCollideWithObstacle( m_theMushee->GetBoundingBox(), m_currentPlayerLayer ) )
				{
					m_theMushee->setLifespan( 0.1f );
					m_theMushee = nullptr;
				}
				ClampMusheeBasedOnLayer();
				m_playerScore += m_increaseScoreRate;
			}
			else
			{
				if( m_respawnTimer < 0.0f)
				{
					SpawnMushee();
					m_respawnTimer = m_respawnTime;
				}
				else
				{
					m_respawnTimer -= DELTA_TIME;
				}

			}
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

		matStackf modelStack;
		modelStack.loadIdentity();
		modelStack.translate( m_bgPosition );
		m_bgParams.set( "modelMatrix", modelStack.top() );

		fireEvent( "renderMesh", m_bgParams );

		m_randomMap->Render();
		for( int i = 0; i < m_currentPlayerLayer; ++i )
		{
			m_randomMap->RenderLayer( i );
		}
		m_actorManager.renderActors();
		for( int i = m_currentPlayerLayer; i <= m_randomMap->GetBottomLayer(); ++i )
		{
			m_randomMap->RenderLayer( i );
		}

		renderHUD();
		//renderDebugHUD();
		//m_GDOManager->renderGDOs( m_renderer, m_frameClock.getElapsedSecondsFloat() );
		m_renderer->popCamera();
		ProfileSystem::getInstance()->clearFrameData();
	}

	void MSGame::renderHUD()
	{
		if( m_hudCamera )
		{
			m_renderer->pushCamera( m_hudCamera );
			for( int i = 0; i < m_buttons.size(); ++i )
			{
				if( m_buttons[i] )
				{
					m_buttons[i]->Render();
				}
			}
			if( m_isPaused )
			{
				m_gameMessageTextParams.set( "text", std::string( "Paused" ) );
				fireEvent( "renderText", m_gameMessageTextParams );
			}
			else if( m_theMushee == nullptr )
			{
				mat4f modelMatrix( mat4f::IDENTITY );
				modelMatrix.translate( vec3f( m_screenWidth * 0.5f, m_screenHeight * 0.5f ) );
				m_gameMessageTextParams.set( "modelMatrix", modelMatrix );
				m_gameMessageTextParams.set( "text", std::string( "Silly Mushee... Be Faster!" ) );
				fireEvent( "renderText", m_gameMessageTextParams );
				modelMatrix.loadIdentity();
				modelMatrix.translate( vec3f( m_screenWidth * 0.5f, m_screenHeight * 0.5f - 96.0f ) );
				m_gameMessageTextParams.set( "modelMatrix", modelMatrix );
				m_gameMessageTextParams.set( "text", std::string( "Respawn in: " ) + toString( (int)m_respawnTimer ) );

				fireEvent( "renderText", m_gameMessageTextParams );
			}
			else
			{
				m_scoreTextParams.set( "text", std::string( "Score: " + toString( (int)m_playerScore ) ) );
				fireEvent( "renderText", m_scoreTextParams );
			}
			m_renderer->popCamera();
		}
	}

	void MSGame::SpawnMushee()
	{
		m_theMushee = new SpriteActor("Mushee", 200.0f, "MusheesRunMat", 10.0f );
		SpriteAnimation* animation = new SpriteAnimation( "animations/MusheesYellow.xml" );
		m_theMushee->AddAnimation( "Run", animation );
		m_theMushee->PlayAnimation( "Run" );
		spawn( m_theMushee );
		m_theMushee->setPosition( m_randomMap->GetPlayerSpawn() );
		m_currentPlayerLayer = m_randomMap->GetMiddleLayer();
		m_randomMap->ResetMap();
		m_playerScore = 0;
	}

	void MSGame::ClampMusheeBasedOnLayer()
	{
		if( m_theMushee != nullptr )
		{
			float minYForLayer = m_randomMap->GetMinYPosForLayer( m_currentPlayerLayer );
			if( m_theMushee->getPosition().y > minYForLayer )
			{
				vec3f pos = m_theMushee->getPosition();
				pos.y = minYForLayer;
				m_theMushee->setPosition( pos );
			}
		}
	}

}
