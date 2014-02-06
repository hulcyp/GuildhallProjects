#include "App.h"
#include "FileManager.h"
#include "MaterialEditorCamera.h"
#include "StartupCommandsParser.h"
#include "MaterialEditorCamera.h"

using namespace Monky;

const double DELTA_TIME = 1/60.0;
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;
const float FOV = 45.0f;

GameApp* g_app = nullptr;

int main( int argc, char** argv )
{
	try
	{
		getFileManager().initialize( "./data/", "data.zip", FileManager::DATA_ARCHIVE_THEN_FOLDER, "supersecretpassword" );
		StartupCommandsParser commandLineParser( argc, argv );
		commandLineParser.issueStartupCommands();

		g_app = new App( DELTA_TIME, WINDOW_WIDTH, WINDOW_HEIGHT, FOV, "Engine Tester" );
		Camera* camera = new MaterialEditorCamera( "MainCamera", vec3f( 10.0f, 10.0f, 20.f ), vec3f(),
			mat4f::perspective( FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 10000.0f ) );
		g_app->initializeCamera( camera );

		g_app->runApp();
	}
	catch( MonkyException& e )
	{
		e.printWhat();
	}
	catch( std::exception& e )
	{
		MonkyException::fatalErrorMessageBox( "Monky Exception", e.what() );
	}

	if( g_app != nullptr )
		g_app->cleanup();

	return 0;
}