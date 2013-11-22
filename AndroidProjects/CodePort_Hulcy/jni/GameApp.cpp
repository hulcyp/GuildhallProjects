#include "GameApp.h"
#include "MonkyRenderer/Renderer.h"
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "CoreEngine/StringUtils.h"
#include "CoreEngine/XMLParser.h"
#include "CoreEngine/TimeUtils.h"
#include "CoreEngine/MonkyKeys.h"
#include "CoreEngine/InputSystem.h"
#include "MonkyRenderer/Actor.h"
#include "MonkyRenderer/MeshFactory.h"
#include "MonkyRenderer/Shader.h"
#include "MonkyRenderer/ShaderProgram.h"


namespace Monky
{
	GameApp::GameApp( double deltaTime, int width, int height, float fov )
		:	Singleton( this )
		,	DELTA_TIME( deltaTime )
		,	m_renderAxis( true )
		,	m_renderDebugHUD( true )
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
		,	m_fov( fov )
		, 	m_isInitialized( false )
	{	
	}
	//-------------------------------------------------------------------------------
	GameApp::~GameApp()
	{
		cleanup();
	}
	//-------------------------------------------------------------------------------
	void GameApp::initialize()
	{
		m_isInitialized = true;
		consolePrintf( "Creating renderer" );
		m_renderer = new Renderer( m_screenWidth, m_screenHeight, m_fov, 0.1f, 100000.0f );
		m_renderer->loadRendereringDataFiles();

		if( m_renderer != nullptr )
		{
				m_debugCamera = new Camera( "DebugCamera", vec3f(), Camera::PH_ORTHOGRAPHIC, mat4f::ortho( 0.0f, (float)m_screenWidth, (float)m_screenHeight, 0.0f, 1.0f, -1.0f ) );
		}

		executeStartUpCmds( "xml/StartupCmds.xml" );

		
		m_mainFontParams.set( "fontName", std::string( "mainFont_72" ) );
		m_mainFontParams.set( "fontHeight", 20.0f );
		m_mainFontParams.set( "color", color::GREEN );
		m_mainFontParams.set( "posRel", std::string( "TOPLEFT" ) );
		m_mainFontParams.set( "camera", m_debugCamera );

		matStackf modelStack;
		modelStack.translate( 0.0f, (float)m_screenHeight - 108, 0.0f );
		m_memoryVisualizerParams.set( "modelMatrix", modelStack.top() );

		//m_spinningCube = spawn( "random actor", MeshFactory::generate2DOrthoRectangle( 20.0f, 20.0f ) );
		//m_spinningCube->setPosition( vec3f( m_screenWidth / 2, m_screenHeight / 2, 0.0f ) );

		SpriteActor* spriteActor = new SpriteActor("Mushee", 5.0f, "SimpleColorMaterial" );
		spawn( spriteActor );
		spriteActor->setPosition( vec3f( m_screenWidth / 2, m_screenHeight / 2, 0.0f ) );
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
		updateFPS();

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
	Actor* GameApp::spawn( const std::string& id, Mesh* mesh )
	{
		Actor* actor = new Actor( id, mesh );
		m_actorManager.addActor( actor );
		return actor;
	}
	//-------------------------------------------------------------------------------
	void GameApp::spawn( SpriteActor* actor )
	{
		m_actorManager.addActor( actor );
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
	//--------------------------------------------------------------------------------
	void GameApp::advanceAppClock( double time )
	{
		m_applicationClock.advance( time );
	}
	//--------------------------------------------------------------------------------
	float GameApp::getElapsedAppTimeSeconds() const
	{
		return m_applicationClock.getElapsedSecondsFloat();
	}
	void GameApp::reloadRenderingAssets()
	{
		m_renderer->initialize();
		Shader::reloadShaders();
		ShaderProgram::reloadShaderPrograms();
		Material::reloadMaterials();
		Texture::ReloadToOpenGL();
		m_actorManager.reloadActorMeshes();
	}
	//-------------------------------------------------------------------------------
	// Protected member functions
	//-------------------------------------------------------------------------------
	void GameApp::updateSimulation()
	{
		m_frameClock.advance( DELTA_TIME );
		TimeUtils::m_currentApplicationTime = m_frameClock.getElapsedSecondsFloat();
		Material::updateTimeOnMaterials( m_frameClock.getElapsedSecondsFloat() );
		m_actorManager.update( DELTA_TIME );
		mat3f rot( mat3f::IDENTITY );
		rot.rotateZ( m_frameClock.getElapsedSecondsFloat() );
		m_spinningCube->setRotation( rot );
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
		m_actorManager.renderActors();
		renderDebugHUD();
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
			m_mainFontParams.set( "text", std::string( "FPS: " ) + floatToString( m_currentFPS, 0 ) );
			mat4f modelMatrix( mat4f::IDENTITY );
			modelMatrix.translate( vec3f( (float)m_screenWidth, (float) m_screenHeight - 30.0f, 0.0f ) );
			m_mainFontParams.set( "modelMatrix", modelMatrix );
			m_mainFontParams.set( "posRel", std::string( "TOPRIGHT" ) );
			fireEvent( "renderText", m_mainFontParams );
			
			//m_mainFontParams.set( "posRel", std::string( "TOPLEFT" ) );
			//modelMatrix.loadIdentity();
			//modelMatrix.translate( vec3f( 0.0f, (float)m_screenHeight ) );
			//m_mainFontParams.set( "modelMatrix", modelMatrix );
			ProfileSystem::getInstance()->renderData( vec3f( 50.0f, (float)m_screenHeight - 30.0f, 0.0f ), m_debugCamera );
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
