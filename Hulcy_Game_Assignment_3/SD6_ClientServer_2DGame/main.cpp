#include "ServerApp.h"
#include "ClientApp.h"
#include "App.h"
#include <FileManager.h>
#include "Camera.h"
#include "SocketUDP.h"
#include "StartupCommandsParser.h"
#include "ClientServerInterface.h"

using namespace Monky;

const double DELTA_TIME = 1/60.0;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float FOV = 45.0f;

void RegisterStartupCommands();
void StartServer( NamedProperties& params );
void StartClient( NamedProperties& params );
void SetClientPort( NamedProperties& params );
void SetServerAddress( NamedProperties& params );

GameApp* g_app = nullptr;
int g_ClientPort = CLIENT_PORT;
std::string g_serverAddr = "localhost";
bool g_triedToStartServer = false;

int main( int argc, char** argv )
{
	try
	{

		getFileManager().initialize( "./data/", "data.zip", FileManager::DATA_ARCHIVE_THEN_FOLDER, "supersecretpassword" );
		RegisterStartupCommands();
		StartupCommandsParser commandLineParser( argc, argv );
		commandLineParser.issueStartupCommands();

		if( g_app == nullptr && !g_triedToStartServer )
		{
			NamedProperties params;
			StartClient( params );
	
		}

		if( g_app != nullptr )
		{
			Camera* camera = new Camera( "Main Camera", vec3f(), Camera::PH_ORTHOGRAPHIC, 
				mat4f::ortho( 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f ) );
			g_app->initializeCamera( camera );

			g_app->runApp();
		}

	}
	catch ( MonkyException& e )
	{
		e.printWhat();
	}
	catch ( std::exception& e )
	{
		MonkyException::fatalErrorMessageBox( "Monky Exception", e.what() );
	}
	g_app->cleanup();
	return 0;
}

void RegisterStartupCommands()
{
	registerForEvent( "server", StartServer );
	registerForEvent( "port", SetClientPort );
	registerForEvent( "serverAddr", SetServerAddress );
}
//----------------------------------------------------------------------
void StartServer( NamedProperties& params )
{
	std::vector< std::string > args;
	params.get( "cmdArgs", args );
	int port = SERVER_PORT;

	if( args.size() > 0 )
	{

		g_app = new ServerApp( args[0], port, 
			DELTA_TIME,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			FOV,
			"Space Shooter Server" );
	}
	else
	{
		consolePrintf( "Must specify ip address for server to bind to" );
	}
}
//----------------------------------------------------------------------
void StartClient( NamedProperties& params )
{
	params;
	ClientApp* app = new ClientApp( DELTA_TIME,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		FOV,
		"Space Shooter Client" );
	if( !app->ConnectToServer( g_serverAddr, g_ClientPort, SERVER_PORT ) )
	{
		consolePrintColorf( "Failed to connect to server", color::RED );
	}
	g_app = (GameApp*)app;
}
//----------------------------------------------------------------------
void SetClientPort( NamedProperties& params )
{
	std::vector< std::string > args;
	params.get( "cmdArgs", args );
	int port = CLIENT_PORT; 
	
	if( args.size() > 0 )
	{
		if( stringToType( args[0], &port ) )
			g_ClientPort = port;
	}
}
//----------------------------------------------------------------------
void SetServerAddress( NamedProperties& params )
{
	std::vector< std::string > args;
	params.get( "cmdArgs", args );
	if( args.size() > 0 )
	{
		g_serverAddr = args[0];
	}
}
