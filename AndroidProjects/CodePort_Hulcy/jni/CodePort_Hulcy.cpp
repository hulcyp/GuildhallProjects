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

#define LOG_TAG "CodePort_Hulcy"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

using namespace Monky;

const double DELTA_TIME = 1/60.0;
const float FOV = 45.0f;




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

	//
	// Here, the application chooses the configuration it desires. In this
	// sample, we have a very simplified selection process, where we pick
	// the first EGLConfig that matches our criteria.
	//

	eglChooseConfig( display, attribs, &config, 1, &numConfigs );

	//
	// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	// guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	// As soon as we picked a EGLConfig, we can safely reconfigure the
	// ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
	//

	eglGetConfigAttrib( display, config, EGL_NATIVE_VISUAL_ID, &format );

	ANativeWindow_setBuffersGeometry( engine->app->window, 0, 0, format );

	surface = eglCreateWindowSurface( display, config, engine->app->window, NULL );

	const EGLint attrib_list [] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	context = eglCreateContext( display, config, NULL, attrib_list );

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

void testEvent( NamedProperties& params )
{
	LOGI( "Test event worked!" );
}

void android_main( struct android_app* app )
{
	app_dummy();

	AndroidInfo androidInfo;
	GameApp* theApp;


	memset( &androidInfo, 0, sizeof(androidInfo) );
	androidInfo.app = app;

	initGL( &androidInfo );

	getFileManager().initialize( androidInfo.app->activity->assetManager );

	registerForEvent( "TestEvent", &testEvent );
	androidInfo.app->activity->assetManager;


	theApp = new GameApp( DELTA_TIME,
						androidInfo.width,
						androidInfo.height,
						FOV,
						&androidInfo);

	fireEvent( "TestEvent" );

	theApp->runApp();


}
