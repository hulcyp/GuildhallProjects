
#include "SDLApp.h"
#ifdef _WIN32
#include <glew.h>
#endif
#include <Gl/GL.h>
#include "Assertion.h"

namespace Monky
{
#pragma warning( disable:4355 )
	SDLApp::SDLApp( int width, int height, float fov, const char* windowTitle )
		:	Singleton( this )	
		,	m_screenWidth( width )
		,	m_screenHeight( height )
		,	m_applicationClock( nullptr )
		,	m_nextTimeFrameIsOwed( 0.0f )
		,	m_frameOwed( true )
		,	m_windowTitle( windowTitle )
	{
		//const SDL_VideoInfo* info = nullptr;
		//int bpp = 0;
		//int flags = 0;
		
		if( windowTitle == nullptr )
		{
			assertion( SDL_Init( 0 ) != -1, "Could not initialize SDL %s.", SDL_GetError() );
		}
		else
		{
			assertion( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != -1, "Could not initialize SDL %s.", SDL_GetError() );
		

			//info = SDL_GetVideoInfo();

			//assertion( info != nullptr, "Video query failed: %s", SDL_GetError() );

			//bpp = info->vfmt->BitsPerPixel;

			SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
			SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
			SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
			SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

			//flags = SDL_OPENGL;

			m_window = SDL_CreateWindow( windowTitle, 
													SDL_WINDOWPOS_CENTERED,
													SDL_WINDOWPOS_CENTERED,
													width, height, SDL_WINDOW_OPENGL );
															
			SDL_GL_CreateContext( m_window );
			//assertion( SDL_SetVideoMode( width, height, bpp, flags ) != 0, "Video mode set failed: %s", SDL_GetError() );

			//SDL_WM_SetCaption( m_windowTitle, "" );
		

#ifdef _WIN32
			glewInit();
#endif
		}
	}
#pragma warning( default:4355 )
	//------------------------------------------------
	SDLApp::~SDLApp()
	{
		SDL_Quit();
	}
	//------------------------------------------------
	void SDLApp::runApp()
	{
		try
		{
			while( true )
			{

				double startTime = TimeUtils::GetAbsoluteTimeSeconds();
						
				if( m_frameOwed )
				{
					updateFrame();
					if( m_window != nullptr )
						SDL_GL_SwapWindow(m_window);
				}
				double endTime = TimeUtils::GetAbsoluteTimeSeconds();		
				m_applicationClock.advance( endTime - startTime );
				if( m_nextTimeFrameIsOwed < m_applicationClock.getElapsedSecondsFloat() )
					m_frameOwed = true;
			}
		}
		catch( QuitProgram& qe )
		{
			qe;
		}
	}
	//------------------------------------------------
	void SDLApp::exitProgram( int code )
	{
		if( code == 0 )
			throw QuitProgram();
		else
		{
			cleanup();
			exit( code );
		}
	}
	//------------------------------------------------
	void SDLApp::warpPointer( int x, int y )
	{
		SDL_WarpMouseInWindow( m_window, x, y );
	}
	//------------------------------------------------
	void SDLApp::showCursor( bool show )
	{
		SDL_ShowCursor( (int)show );
	}
	//------------------------------------------------
	int SDLApp::getModifiers()
	{
		return InputSystem::getInstance()->getCurrentModifiers();
	}
	//------------------------------------------------
	void SDLApp::processSystemEvents()
	{
		while( SDL_PollEvent( &m_evtStruct ) )
		{
			switch( m_evtStruct.type )
			{
			case SDL_KEYDOWN:
				handleKeyDownEvent( &m_evtStruct.key.keysym );
				break;
			case SDL_KEYUP:
				handleKeyUpEvent( &m_evtStruct.key.keysym );
				break;
			case SDL_MOUSEMOTION:
				onMouseMove( m_evtStruct.motion.x, m_evtStruct.motion.y );
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				handleMouseButtonEvent( &m_evtStruct );
				break;
			case SDL_DROPFILE:
				handleFileDrop( &m_evtStruct.drop );
				break;
			case SDL_QUIT:
				exitProgram( 0 );
				break;			
			}
		}
	}
	//------------------------------------------------
	void SDLApp::handleKeyDownEvent( SDL_Keysym* keysym )
	{
		int x = 0, y = 0;
		SDL_GetMouseState( &x, &y );
		InputSystem::getInstance()->setCurrentModifiers( (MonkyMod)keysym->mod );		
		onKeyDown( (MonkyKey)keysym->sym, x, y );
	}
	//------------------------------------------------
	void SDLApp::handleKeyUpEvent( SDL_Keysym* keysym )
	{
		int x = 0, y = 0;
		SDL_GetMouseState( &x, &y );
		InputSystem::getInstance()->setCurrentModifiers( (MonkyMod)keysym->mod );		
		onKeyUp( (MonkyKey)keysym->sym, x, y );
	}
	//------------------------------------------------
	void SDLApp::handleMouseButtonEvent( SDL_Event* buttonEvent )
	{
		int x = 0, y = 0;
		SDL_GetMouseState( &x, &y );
		InputSystem::getInstance()->setCurrentModifiers( (MonkyMod) (buttonEvent->key.keysym.mod) );
		onMouseButton( buttonEvent->button.button, (MonkyMouseButtonState)buttonEvent->button.state, x, y );
	}
	//------------------------------------------------
	void SDLApp::handleFileDrop( SDL_DropEvent* event )
	{
		onFileDrop( std::string( event->file ) );
	}
	
}