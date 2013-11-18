#include "GameApp.h"
#include "MonkyRenderer/Renderer.h"
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "CoreEngine/StringUtils.h"
#include "CoreEngine/XMLParser.h"
#include "CoreEngine/TimeUtils.h"
#include "CoreEngine/MonkyKeys.h"
#include "CoreEngine/InputSystem.h"


namespace Monky
{
	GameApp::GameApp( double deltaTime, int width, int height, float fov, AndroidInfo* androidInfo )
		:	Singleton( this )
		,	DELTA_TIME( deltaTime )
		,	m_renderAxis( false )
		,	m_renderDebugHUD( false )
		,	m_frameClock( nullptr )
		,	m_currentFPS( 0.0f )
		,	m_previousTime( 0.0f )
		,	m_frameCount( 0 )
		,	m_debugCamera( nullptr )
		,	m_renderer( nullptr )
		,	m_camera( nullptr )
		,	m_screenWidth( width )
		,	m_screenHeight( height )
		,	m_applicationClock( nullptr )
		,	m_androidInfo( androidInfo )
	{	


		m_renderer = new Renderer( m_screenWidth, m_screenHeight, fov, 0.1f, 100000.0f );
		m_renderer->loadRendereringDataFiles();

		initialize();
	}
	//-------------------------------------------------------------------------------
	GameApp::~GameApp()
	{
		cleanup();
	}
	//-------------------------------------------------------------------------------
	void GameApp::runApp()
	{
		try
		{
			while( true )
			{

				double startTime = TimeUtils::GetAbsoluteTimeSeconds();

				if( m_frameOwed )
				{
					updateFrame();
					eglSwapBuffers( m_androidInfo->display, m_androidInfo->surface );
				}
				double endTime = TimeUtils::GetAbsoluteTimeSeconds();
				m_applicationClock.advance( endTime - startTime );
				if( m_nextTimeFrameIsOwed < m_applicationClock.getElapsedSecondsFloat() )
					m_frameOwed = true;
			}
		}
		catch( QuitProgram& qe )
		{
			//qe;
		}
	}
	//-------------------------------------------------------------------------------
	void GameApp::initialize()
	{
		if( m_renderer != nullptr )
		{
			//m_font = Font::getFont( "mainFont_72" );
			m_debugCamera = new Camera( "DebugCamera", vec3f(), Camera::PH_ORTHOGRAPHIC, mat4f::ortho( 0.0f, (float)m_screenWidth, (float)m_screenHeight, 0.0f, 1.0f, -1.0f ) );
		}

		executeStartUpCmds( "xml/StartupCmds.xml" );

		/*
		registerConsoleCommand( "event", "Fire event. Usage: [event name]",
			[&]( const std::string& args )
		{
			fireEvent( args );
		} );
		registerConsoleCommand( "exit", "Exit the program", []( const std::string& ){ GameApp::getInstance()->exitProgram( 0 ); } );
		registerConsoleCommand( "quit", "Quit the program", []( const std::string& ){ GameApp::getInstance()->exitProgram( 0 ); } );
		*/
		
		m_mainFontParams.set( "fontName", std::string( "mainFont_72" ) );
		m_mainFontParams.set( "fontHeight", 12.0f );
		m_mainFontParams.set( "color", color::GREEN );
		m_mainFontParams.set( "posRel", std::string( "TOPLEFT" ) );

		matStackf modelStack;
		modelStack.translate( 0.0f, (float)m_screenHeight - 108, 0.0f );
		m_memoryVisualizerParams.set( "modelMatrix", modelStack.top() );
	}
	//-------------------------------------------------------------------------------
	void GameApp::cleanup()
	{
		SAFE_DELETE( m_renderer );
		SAFE_DELETE( m_camera );	
		SAFE_DELETE( m_debugCamera );
		//cleanupGDOManager();
		cleanupConsole();
		//m_GDOManager = nullptr;
	}
	//-------------------------------------------------------------------------------
	void GameApp::updateFrame()
	{	
		//processSystemEvents();
		updateSimulation();
		updateDisplay();		
		m_frameOwed = false;	
		updateFPS();	
		
		if( !m_frameOwed )
		{
			m_nextTimeFrameIsOwed = m_applicationClock.getElapsedSecondsFloat() + (float)DELTA_TIME;
		}
		ProfileSystem::getInstance()->clearFrameData();
	}
	//-------------------------------------------------------------------------------
	bool GameApp::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		
		InputSystem::getInstance()->onKeyDown( keyCode, x, y );

		return used;
	}
	//-------------------------------------------------------------------------------
	bool GameApp::onKeyUp( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		InputSystem::getInstance()->onKeyUp( keyCode, x, y );\
		return used;
	}
	//-------------------------------------------------------------------------------
	void GameApp::onMouseMove( int mouseX, int mouseY )
	{
		InputSystem::getInstance()->onMouseMove( mouseX, mouseY );
	}
	//-------------------------------------------------------------------------------
	bool GameApp::onMouseButton( int keyCode, MonkyMouseButtonState state, int mouseX, int mouseY )
	{
		bool used = false;
		InputSystem::getInstance()->onMouseButton( keyCode, state, mouseX, mouseY );
		return used;
	}
	//-------------------------------------------------------------------------------
	void GameApp::executeStartUpCmds( const std::string& filePath )
	{
		XMLParser parser( filePath.c_str(), false );
		XMLDocument& doc = parser.getDocument();
		if( !doc.Error() )
		{
			const XMLNode* root = doc.FirstChildElement( "StartupCommands" );
			parser.validateXMLChildElements( root, "Command", "" );
			for( const XMLNode* command = root->FirstChildElement( "Command" ); command != nullptr; command = command->NextSiblingElement( "Command" ) )
			{
				std::string pcData = parser.getXMLElementPCDataAsString( command );
				//if( pcData.size() > 0 )
				//	m_console->executeCommand( pcData );
			}
		}
	}
	//-------------------------------------------------------------------------------
	void GameApp::initializeCamera( Camera* camera )
	{
		if( camera != nullptr && m_renderer != nullptr )
		{
			m_camera = camera;
			m_renderer->pushCamera( m_camera );
		}
	}

	//-------------------------------------------------------------------------------
	// Protected member functions
	//-------------------------------------------------------------------------------
	void GameApp::updateSimulation()
	{
		m_frameClock.advance( DELTA_TIME );
		Material::updateTimeOnMaterials( m_frameClock.getElapsedSecondsFloat() );
		//m_actorManager.update( DELTA_TIME );
	}
	//-------------------------------------------------------------------------------
	void GameApp::updateDisplay()
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
		//m_actorManager.renderActors();
		//m_GDOManager->renderGDOs( m_renderer, m_frameClock.getElapsedSecondsFloat() );
		m_renderer->popCamera();
		ProfileSystem::getInstance()->clearFrameData();		
	}
	//-------------------------------------------------------------------------------
	void GameApp::renderHUD()
	{}
	//-------------------------------------------------------------------------------
	void GameApp::renderDebugHUD()
	{
		if( m_renderDebugHUD )
		{
			m_renderer->pushCamera( m_debugCamera );
			m_mainFontParams.set( "text", "FPS: " + floatToString( m_currentFPS, 0 ) );
			mat4f modelMatrix( mat4f::IDENTITY );
			modelMatrix.translate( vec3f( (float)m_screenWidth, (float) m_screenHeight ) );
			m_mainFontParams.set( "modelMatrix", modelMatrix );
			m_mainFontParams.set( "posRel", std::string( "TOPRIGHT" ) );
			fireEvent( "renderText", m_mainFontParams );
			
			m_mainFontParams.set( "posRel", std::string( "TOPLEFT" ) );
			modelMatrix.loadIdentity();
			modelMatrix.translate( vec3f( 0.0f, (float)m_screenHeight ) );
			m_mainFontParams.set( "modelMatrix", modelMatrix );
			ProfileSystem::getInstance()->renderData( vec3f( 50.0f, (float)m_screenHeight, 0.0f ), m_debugCamera );
			m_renderer->popCamera();
		}
	}
	//-------------------------------------------------------------------------------
	void GameApp::renderConsole()
	{
		//if( m_console )
		//{
		//	m_console->renderConsole();
		//}
	}
	//-------------------------------------------------------------------------------
	void GameApp::updateFPS()
	{
		++m_frameCount;
		
		float timeInterval = m_applicationClock.getElapsedSecondsFloat() - m_previousTime;

		if( timeInterval > 0.1f )
		{
			m_currentFPS = (float)( m_frameCount / timeInterval );
			m_previousTime = m_applicationClock.getElapsedSecondsFloat();
			m_frameCount = 0;
		}
	}
}
