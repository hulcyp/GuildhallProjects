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
#include "Game/MSGame.h"
#include "MonkyRenderer/Camera.h"
#include "CoreEngine/Util/StringUtils.h"
#include "Game/MSCamera.h"

#define LOG_TAG "CodePort_Hulcy"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

using namespace Monky;

const double DELTA_TIME = 1/60.0;
const float FOV = 45.0f;


bool g_frameOwed = true;
double g_nextTimeFrameIsOwed = 0.0;
MSGame* g_app = nullptr;
AndroidInfo g_androidInfo;


static void Update();
static int initGL();
static void shutdownGL();
static void initGame();


static void printGLString( const char *name, GLenum s )
{
	const char *v = (const char *) glGetString( s );
	LOGI( "GL %s = %s\n", name, v );
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
			if( engine->app->window != nullptr )
			{
				consolePrintf( "Calling: initGame()" );
				initGame();
			}
		}
		break;
	case APP_CMD_TERM_WINDOW:
		{
			// The window is being hidden or closed, clean it up.
			shutdownGL();
		}
		break;
	case APP_CMD_GAINED_FOCUS:
		{
			g_androidInfo.isPaused = false;
			initGL();
			g_app->reloadRenderingAssets();
			consolePrintf( "Gained focus" );
		}
		break;
	case APP_CMD_LOST_FOCUS:
		{
			g_androidInfo.isPaused = true;
			consolePrintf( "Lost focus" );
		}
		break;
	}
}

static int32_t engine_handle_input( struct android_app* app, AInputEvent* event )
{
	if( g_app != nullptr && g_app->isInitialized() )
	{
		if( AInputEvent_getType( event ) == AINPUT_EVENT_TYPE_MOTION )
		{
			int fingerId = AMotionEvent_getPointerId( event, 0 );
			float x = AMotionEvent_getX( event, 0 );
			float y = AMotionEvent_getY( event, 0 );

			if( AMotionEvent_getAction( event ) == AMOTION_EVENT_ACTION_DOWN )
			{
				g_app->onFingerDown( fingerId, x, y );
			}
			else if( AMotionEvent_getAction( event ) == AMOTION_EVENT_ACTION_UP )
			{
				g_app->onFingerUp( fingerId, x, y );
			}
		}
	}
}


void android_main( struct android_app* app )
{
	app_dummy();

	memset( &g_androidInfo, 0, sizeof(g_androidInfo) );
	g_androidInfo.app = app;
	app->userData = &g_androidInfo;
	app->onAppCmd = handleAppCmd;
	app->onInputEvent = engine_handle_input;

	g_androidInfo.isPaused = false;

	g_androidInfo.sensorManager = ASensorManager_getInstance();
	g_androidInfo.accelerometerSensor = ASensorManager_getDefaultSensor( g_androidInfo.sensorManager, ASENSOR_TYPE_ACCELEROMETER );
	g_androidInfo.sensorEventQueue = ASensorManager_createEventQueue( g_androidInfo.sensorManager,app->looper, LOOPER_ID_USER, NULL, NULL );

	consolePrintf( "Initialzing file manager" );
	getFileManager().initialize( g_androidInfo.app->activity->assetManager );

	std::string testStr = "   test   ";
	stripLeadingAndTrailingWhiteSpace( testStr );
	consolePrintf( "TEST STRING: %s", testStr.c_str() );

	while( true )
	{
		int ident;
		int events;
		struct android_poll_source* source;

		while( ident = ALooper_pollAll( 0, nullptr, &events, (void**)&source ) >= 0 )
		{
			if( source != nullptr )
				source->process( app, source );

			if( ident == LOOPER_ID_USER )
			{
				ASensorEvent event;

				while( ASensorEventQueue_getEvents( g_androidInfo.sensorEventQueue, &event, 1 ) > 0 )
				{

				}
			}

			// Check if we are exiting.
			if( app->destroyRequested != 0 )
			{
				shutdownGL();
				return;
			}
		}

		Update();
	}
}


void Update()
{
	if( !g_androidInfo.isPaused && g_app != nullptr && g_app->isInitialized() )
	{
		double startTime = TimeUtils::GetAbsoluteTimeSeconds();

		if( g_frameOwed )
		{
			//consolePrintf( "Frame owed" );
			g_app->updateFrame();
			g_frameOwed = false;
			eglSwapBuffers( g_androidInfo.display, g_androidInfo.surface );
			g_nextTimeFrameIsOwed = g_app->getElapsedAppTimeSeconds() + (float)DELTA_TIME;

		}
		double endTime = TimeUtils::GetAbsoluteTimeSeconds();
		//consolePrintf( "Frame time: %f", endTime - startTime );
		//consolePrintf( "Next Time Frame is Owed: %f", g_nextTimeFrameIsOwed );
		//consolePrintf( "Elapsed App Time: %f", g_app->getElapsedAppTimeSeconds() );
		g_app->advanceAppClock( endTime - startTime );
		//if( g_nextTimeFrameIsOwed < g_app->getElapsedAppTimeSeconds() )
		g_frameOwed = true;
	}
}

static void initGame()
{
	consolePrintf( "Game App Created...");
	consolePrintf( "Initializing...");
	if( g_app == nullptr )
	{
		initGL();
		g_app = new MSGame( DELTA_TIME,
										g_androidInfo.width,
										g_androidInfo.height,
										FOV );

		g_app->initialize();
		Camera* camera = new MSCamera( "Main Camera", vec3f(), Camera::PH_ORTHOGRAPHIC,
						mat4f::ortho( 0.0f, (float)g_androidInfo.width, 0.0f, (float)g_androidInfo.height, -1.0f, 1.0f ) );
		g_app->initializeCamera( camera );
	}
}

static int initGL()
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

	ANativeWindow_setBuffersGeometry( g_androidInfo.app->window, 0, 0, format );
	consolePrintf( "Set buffer geometry" );

	surface = eglCreateWindowSurface( display, config, g_androidInfo.app->window, NULL );
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

	g_androidInfo.display = display;
	g_androidInfo.context = context;
	g_androidInfo.surface = surface;
	g_androidInfo.width = w;
	g_androidInfo.height = h;
	consolePrintf( "Dim: (%d,%d)", w, h );

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




void shutdownGL()
{
	if( g_androidInfo.display != EGL_NO_DISPLAY )
	{
		eglMakeCurrent( g_androidInfo.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );

		if( g_androidInfo.context != EGL_NO_CONTEXT )
			eglDestroyContext( g_androidInfo.display, g_androidInfo.context );

		if( g_androidInfo.surface != EGL_NO_SURFACE )
			eglDestroySurface( g_androidInfo.display, g_androidInfo.surface );

		eglTerminate( g_androidInfo.display );
	}

	g_androidInfo.display = EGL_NO_DISPLAY;
	g_androidInfo.context = EGL_NO_CONTEXT;
	g_androidInfo.surface = EGL_NO_SURFACE;
	consolePrintf( "openGL shutdown" );
}
