#include <jni.h>
#include <android_native_app_glue.h>
#include <android/sensor.h>
#include <android/log.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <functional>

#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "CoreEngine/FileManager.h"
#include "GameApp.h"
#include "MonkyRenderer/Camera.h"

#define LOG_TAG "CodePort_Hulcy"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

using namespace Monky;

const double DELTA_TIME = 1/60.0;
const float FOV = 45.0f;


bool g_frameOwed = true;
double g_nextTimeFrameIsOwed = 0.0;
GameApp* g_app;
AndroidInfo g_androidInfo;


static void printGLString( const char *name, GLenum s )
{
	const char *v = (const char *) glGetString( s );
	LOGI( "GL %s = %s\n", name, v );
}

static int initGL( AndroidInfo* engine )
{
	// initialize OpenGL ES and EGL

	//
	// Here specify the attributes of the desired configuration.
	// Below, we select an EGLConfig with at least 8 bits per color
	// that supports OpenGL ES 2.0.
	//

	const EGLint attribs[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;


	EGLDisplay display = eglGetDisplay( EGL_DEFAULT_DISPLAY );

	eglInitialize( display, 0, 0 );

	consolePrintf( "Initialzing egl init" );
	//
	// Here, the application chooses the configuration it desires. In this
	// sample, we have a very simplified selection process, where we pick
	// the first EGLConfig that matches our criteria.
	//

	eglChooseConfig( display, attribs, &config, 1, &numConfigs );
	consolePrintf( "Initialzing config chosen" );
	//
	// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	// guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	// As soon as we picked a EGLConfig, we can safely reconfigure the
	// ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
	//

	eglGetConfigAttrib( display, config, EGL_NATIVE_VISUAL_ID, &format );
	consolePrintf( "Initialzing got config attrib" );

	ANativeWindow_setBuffersGeometry( engine->app->window, 0, 0, format );
	consolePrintf( "Set buffer geometry" );

	surface = eglCreateWindowSurface( display, config, engine->app->window, NULL );
	consolePrintf( "Initialzing surface created" );

	const EGLint attrib_list [] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	context = eglCreateContext( display, config, NULL, attrib_list );
	consolePrintf( "Initialzing context created" );

	if( eglMakeCurrent( display, surface, surface, context ) == EGL_FALSE )
	{
		LOGW( "Unable to eglMakeCurrent" );
		return -1;
	}

	eglQuerySurface( display, surface, EGL_WIDTH, &w );
	eglQuerySurface( display, surface, EGL_HEIGHT, &h );

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;

	// Initialize GL state.
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );
	glDisable( GL_DEPTH_TEST );

	printGLString( "Version", GL_VERSION );
	printGLString( "Vendor", GL_VENDOR );
	printGLString( "Renderer", GL_RENDERER );
	printGLString( "Extensions", GL_EXTENSIONS );

	glViewport( 0, 0, w, h );


	return 0;
}

void shutdownGL( struct AndroidInfo* engine )
{
	if( engine->display != EGL_NO_DISPLAY )
	{
		eglMakeCurrent( engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );

		if( engine->context != EGL_NO_CONTEXT )
			eglDestroyContext( engine->display, engine->context );

		if( engine->surface != EGL_NO_SURFACE )
			eglDestroySurface( engine->display, engine->surface );

		eglTerminate( engine->display );
	}

	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

void initGame()
{
	if( g_app == nullptr )
	{
		g_app = new GameApp( DELTA_TIME,
								g_androidInfo.width,
								g_androidInfo.height,
								FOV );
	}
	if( g_app != nullptr && !g_app->isInitialized() )
	{
		initGL( &g_androidInfo );

		g_app->initialize();
		Camera* camera = new Camera( "Main Camera", vec3f(), Camera::PH_ORTHOGRAPHIC,
						mat4f::ortho( 0.0f, (float)g_androidInfo.width, (float)g_androidInfo.height, 0.0f, -1.0f, 1.0f ) );
		g_app->initializeCamera( camera );
	}
}

void handleAppCmd( struct android_app* app, int32_t cmd )
{
	struct AndroidInfo* engine = (struct AndroidInfo*) app->userData;

	switch( cmd )
	{
	case APP_CMD_INIT_WINDOW:
		{
			// The window is being shown, get it ready.
			consolePrintf( "APP_CMD_INIT_WINDOW" );
			if( engine->app->window != NULL )
			{
				initGame();
			}
		}
	case APP_CMD_TERM_WINDOW:
		{
			// The window is being hidden or closed, clean it up.
			shutdownGL( engine );
		}
		break;
	}
}


void testEvent( NamedProperties& params )
{
	LOGI( "Test event worked!" );
}



void android_main( struct android_app* app )
{
	app_dummy();

	memset( &g_androidInfo, 0, sizeof(g_androidInfo) );
	g_androidInfo.app = app;
	app->userData = &g_app;
	app->onAppCmd = handleAppCmd;


	consolePrintf( "Initialzing file manager" );
	getFileManager().initialize( g_androidInfo.app->activity->assetManager );

	registerForEvent( "TestEvent", &testEvent );


	fireEvent( "TestEvent" );

	while( true )
	{
		if( g_app != nullptr && g_app->isInitialized() )
		{
			float startTime = TimeUtils::GetAbsoluteTimeSeconds();

			if( g_frameOwed )
			{
				g_app->updateFrame();
				g_frameOwed = false;
				eglSwapBuffers( g_androidInfo.display, g_androidInfo.surface );
				g_nextTimeFrameIsOwed = g_app->getElapsedAppTimeSeconds() + (float)DELTA_TIME;

			}
			double endTime = TimeUtils::GetAbsoluteTimeSeconds();
			g_app->advanceAppClock( endTime - startTime );
			if( g_nextTimeFrameIsOwed < g_app->getElapsedAppTimeSeconds() )
				g_frameOwed = true;
		}

		// Check if we are exiting.
		if( app->destroyRequested != 0 )
		{
			shutdownGL( &g_androidInfo );
			return;
		}
	}


}
