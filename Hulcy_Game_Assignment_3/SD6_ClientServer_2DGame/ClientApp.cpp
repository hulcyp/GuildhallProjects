#include "ClientApp.h"
#include "SocketUDP.h"
#include "Bullet.h"
#include "Spaceship.h"
#include "ClientServerInterface.h"
#include "CollisionManager.h"
#include "Packet.h"
#include "LocalPlayerController.h"

namespace Monky
{
	ClientApp::ClientApp( double deltaTime, int width, int height, float fov, const char* windowTitle )
		:	GameApp( deltaTime, width, height, fov, windowTitle )
		,	m_serverSocket( nullptr )
		,	m_mostRecentPacketIDRecieved( 0 )
		,	m_mostRecentPacketIDSent( 0 )
		,	m_playerController( nullptr )
	{
		m_collisionManager = new CollisionManager();
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			m_spaceships[i] = nullptr;
		}

		registerConsoleCommand( "connect", "Connect to server. Usage: connect [host name]",
			[&]( const std::string& args )
		{
			std::vector< std::string > tokens;
			stringTokenizer( args, tokens, " " );
			if( tokens.empty() )
			{
				consolePrintColorf( "Invalid usage. Must specify host name", color::RED );
			}
			else
			{
				ConnectToServer( tokens[0], CLIENT_PORT, SERVER_PORT );
			}
		} );
		m_networkPacket = new Packet( PACKET_SIZE );
		m_ackPacket = new Packet( PACKET_SIZE );
	}
	//----------------------------------------------------------------------
	ClientApp::~ClientApp()
	{
	}
	//----------------------------------------------------------------------
	void ClientApp::cleanup()
	{
		GameApp::cleanup();
		if( m_serverSocket != nullptr && m_playerController != nullptr )
		{
			m_networkPacket->AddData( ++m_mostRecentPacketIDSent );
			m_networkPacket->AddData( SERVER_PACKET_QUIT );
			m_networkPacket->AddData( m_playerController->getTeamNumber() );

			m_serverSocket->Send( m_networkPacket, m_serverHostName, m_serverPort );

			for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
			{
				SAFE_DELETE( m_spaceships[i] );
			}
			m_serverSocket->Close();
			SAFE_DELETE( m_serverSocket );
			SAFE_DELETE( m_networkPacket );
			SAFE_DELETE( m_playerController );
		}
	}
	//----------------------------------------------------------------------
	bool ClientApp::ConnectToServer( const std::string& host, int localPort, int serverPort )
	{
		bool success = true;
		m_serverSocket = new SocketUDP( "127.0.0.1", CLIENT_PORT );
		if( !m_serverSocket->IsValidSocket() )
		{
			SAFE_DELETE( m_serverSocket );
			vLog( "Failed to create socket" );
			success = false;
		}
		else
		{
			m_networkPacket->AddData( m_mostRecentPacketIDSent );
			m_networkPacket->AddData( SERVER_PACKET_JOIN );
			consolePrintf( "Requesting to join server: %s:%d...", host.c_str(), serverPort );
			if( m_serverSocket->Send( m_networkPacket, host, SERVER_PORT ) < PACKET_SIZE )
			{
				consolePrintf( "Failed to send data to server" );
				success = false;
				SAFE_DELETE( m_serverSocket );
			}
			else
			{
				m_serverHostName = host;
				m_serverPort = serverPort;
				consolePrintf( "Waiting for reply from server..." );
				m_networkPacket->ResetToBeginning();
				if( m_serverSocket->Recieve( m_networkPacket, 5000 ) < PACKET_SIZE )
				{
					success = false;
					consolePrintColorf( "Failed to connect to server", color::RED );
					SAFE_DELETE( m_serverSocket );
				}
				else
				{
					int packetID = -1;
					ClientPacketType type;
					if( m_networkPacket->GetNextChunkOfData( packetID ) )
					{
						if( packetID > m_mostRecentPacketIDRecieved )
						{
							m_mostRecentPacketIDRecieved = packetID;
							if( m_networkPacket->GetNextChunkOfData( type ) )
							{
								short teamNumber = -1;
								switch( type )
								{
								case CLIENT_PACKET_REPLY_TO_JOIN:
									if( m_networkPacket->GetNextChunkOfData( teamNumber ) )
									{
										//m_teamNumber = teamNumber;
										if( teamNumber != -1 )
										{
											consolePrintColorf( "Joined game as player number: %d", color::YELLOW, teamNumber );
											m_playerController = new LocalPlayerController( teamNumber );

											m_ackPacket->ResetToBeginning();
											m_ackPacket->AddData( packetID );
											m_ackPacket->AddData( SERVER_PACKET_ACK );
											m_ackPacket->AddData( teamNumber );											
											m_serverSocket->ReplyToLastRecieve( m_ackPacket );

											SendUpdateToServer();
										}
										else
										{
											consolePrintColorf( "Sorry the game is already full", color::YELLOW );
											SAFE_DELETE( m_serverSocket );
											success = false;
										}
									}
									break;
								}
							}
							else
							{
								vLog( "Failed to retrieve packet type for response to request to join game" );
							}
						}
					}
				}
			}
		}
		m_networkPacket->ResetToBeginning();
		return success;
	}
	//----------------------------------------------------------------------
	bool ClientApp::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		used = GameApp::onKeyDown( keyCode, x, y );
		if( !used && m_playerController != nullptr )
			m_playerController->onKeyDown( keyCode, x, y );
			//m_keyboard[ keyCode ] = true;
		return true;
	}
	//----------------------------------------------------------------------
	bool ClientApp::onKeyUp( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		used = GameApp::onKeyUp( keyCode, x, y );
		if( !used && m_playerController != nullptr )
			m_playerController->onKeyUp( keyCode, x, y );
			//m_keyboard[ keyCode ] = false;
		return true;
	}	
	//----------------------------------------------------------------------
	void ClientApp::updateSimulation()
	{
		RecieveUpdateFromServer();
		GameApp::updateSimulation();
		m_collisionManager->udpate( DELTA_TIME );
		if( m_playerController != nullptr )
			m_playerController->update( DELTA_TIME );
		SendUpdateToServer();
	}
	//----------------------------------------------------------------------
	void ClientApp::updateDisplay()
	{
		GameApp::updateDisplay();
		GameApp::renderHUD();
		GameApp::renderDebugHUD();
		GameApp::renderConsole();
	}
	//----------------------------------------------------------------------
	void ClientApp::SpawnSpaceship( const std::string& name, short teamNumber, const vec2f& pos )
	{
		if( 0 <= teamNumber && teamNumber < MAX_NUM_PLAYERS && m_spaceships[ teamNumber ] == nullptr )
		{
			m_spaceships[ teamNumber] = new Spaceship( name, teamNumber, m_screenWidth, m_screenHeight );
			m_spaceships[ teamNumber ]->setPosition( pos );
			spawn( m_spaceships[ teamNumber ] );
			if( m_playerController != nullptr && teamNumber == m_playerController->getTeamNumber() )
				m_playerController->assignShip( m_spaceships[ teamNumber ] );
		}
	}
	//----------------------------------------------------------------------
	void ClientApp::SpawnBullet( const vec2f& pos, float rotation, short teamNumber )
	{
		spawn( new Bullet( pos, rotation, teamNumber, m_screenWidth, m_screenHeight ) );
	}
	//----------------------------------------------------------------------
	void ClientApp::SendUpdateToServer()
	{
		if( m_serverSocket != nullptr )
		{
			m_networkPacket->ResetToBeginning();
			m_networkPacket->AddData( ++m_mostRecentPacketIDSent );
			m_networkPacket->AddData( SERVER_PACKET_UPDATE );
			if( m_playerController != nullptr )
				m_networkPacket->AddData( m_playerController->getTeamNumber() );
			else
				m_networkPacket->AddData( (short)-1 );

			if( m_playerController != nullptr )
			{
				std::vector< MonkyKey > keysPressed = m_playerController->GetKeysPressed();
				m_networkPacket->AddData( (int)keysPressed.size() );
				for( int i = 0; i < (int)keysPressed.size(); ++i )
				{
					m_networkPacket->AddData( keysPressed[i] );
				}
				if( m_serverSocket->Send( m_networkPacket, m_serverHostName, m_serverPort ) < PACKET_SIZE )
				{
					consolePrintColorf( "Failed to send update to server", color::RED );
				}
			}
			m_networkPacket->ResetToBeginning();
		}
	}
	//----------------------------------------------------------------------
	bool ClientApp::RecieveUpdateFromServer()
	{
		bool recieved = false;
		if( m_serverSocket != nullptr )
		{
			if( m_serverSocket->Recieve( m_networkPacket, CLIENT_WAIT_TIME_PER_FRAME ) > 0 )
			{
				int packetID = -1;
				ClientPacketType type;
				if( m_networkPacket->GetNextChunkOfData( packetID ) )
				{					
					m_ackPacket->ResetToBeginning();
					m_ackPacket->AddData( packetID );
					m_ackPacket->AddData( SERVER_PACKET_ACK );		
					m_ackPacket->AddData( m_playerController->getTeamNumber() );
					m_serverSocket->ReplyToLastRecieve( m_ackPacket );

					if( packetID > m_mostRecentPacketIDRecieved )
					{
						m_mostRecentPacketIDRecieved = packetID;
						if( m_networkPacket->GetNextChunkOfData( type ) )
						{
							recieved = true;
							switch( type )
							{
							case CLIENT_PACKET_UPDATE:
								HandleServerUpdate();
								break;
							case CLIENT_PACKET_DISCONNECTED:
								HandleDisconnect();
								break;
							case CLIENT_PACKET_GAME_OVER:
								//Handle game over
								break;
							}
						}
					}
					else
					{
						consolePrintColorf( "Recieved earlier package... ignoring", color::YELLOW );
					}
				}
			}
			m_networkPacket->ResetToBeginning();
		}
		return recieved;
	}
	//----------------------------------------------------------------------
	void ClientApp::HandleServerUpdate()
	{
		vec2f pos( -1.0f, -1.0f );
		vec2f vel( -1.0f, -1.0f );
		float rotation = 0.0f;
		int nameLen = 0;
		char name[ MAX_NAME_SIZE ];

		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			vLog( "Handling server update" );
			m_networkPacket->GetNextChunkOfData( pos );
			m_networkPacket->GetNextChunkOfData( vel );
			m_networkPacket->GetNextChunkOfData( rotation );
			m_networkPacket->GetNextChunkOfData( nameLen );
			m_networkPacket->GetBufferOfData( name, nameLen );
			name[ nameLen ] = '\0';

			if( pos.x == -1.0f && m_spaceships[i] != nullptr )
			{
				m_spaceships[i]->setLifespan( 0.1f );
				m_spaceships[i] = nullptr;
			}
			else if( m_spaceships[i] == nullptr && pos.x >= 0.0f )
			{
				SpawnSpaceship( name, i, pos );
			}
			else if( m_spaceships[i] != nullptr && !m_spaceships[i]->isDead() )
			{
				m_spaceships[i]->setPosition( pos );
				m_spaceships[i]->setVeloctiy( vel );
				m_spaceships[i]->setSGARotation( rotation );
			}

		}

		int numBulletsToSpawn = -1;
		m_networkPacket->GetNextChunkOfData( numBulletsToSpawn );
		for( int i = 0; i < numBulletsToSpawn; ++i )
		{
			short teamNumber = -1;
			m_networkPacket->GetNextChunkOfData( teamNumber );
			float rotation = 0.0f;
			m_networkPacket->GetNextChunkOfData( rotation );
			m_networkPacket->GetNextChunkOfData( pos );

			SpawnBullet( pos, rotation, teamNumber );
		}
	}
	//----------------------------------------------------------------------
	void ClientApp::HandleDisconnect()
	{
		m_actorManager.killAll();
		m_serverSocket->Close();
		SAFE_DELETE( m_serverSocket );
		ARRAY_CLEANUP( m_spaceships, MAX_NUM_PLAYERS );
		SAFE_DELETE( m_playerController );
		consolePrintColorf( "Disconnected from server", color::RED );
	}
}