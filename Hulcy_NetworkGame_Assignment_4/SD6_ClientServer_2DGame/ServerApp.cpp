#include "ServerApp.h"
#include "CollisionManager.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "NetworkPlayerController.h"
#include "SocketUDP.h"
#include "ClientServerInterface.h"
#include "RandItemPool.h"
#include "Packet.h"

namespace Monky
{
	ServerApp::ServerApp( const std::string& serverAddr, int serverPort, double deltaTime, int width, int height, float fov, const char* windowTitle )
		:	GameApp( deltaTime, width, height, fov, nullptr, false )
		,	m_gameStarted( false )
		,	m_currentNumPlayers( 0 )
	{
		m_collisionManager = new CollisionManager();
		m_socket = new SocketUDP( serverAddr, serverPort );
		m_networkPacket = new Packet( PACKET_SIZE );
		if( m_socket->Bind() )
		{
			consolePrintf( "Server started..." );
			consolePrintf( "Waiting for players to join..." );
		}
		else
		{
			SAFE_DELETE( m_socket );
		}
		registerForEvent( "SpawnBullet", *this, &ServerApp::SpawnBulletEvt );

		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			m_players[i] = nullptr;
		}
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			m_spaceships[i] = nullptr;	
		}
	}
	//----------------------------------------------------------------------
	ServerApp::~ServerApp()
	{
		LIST_CLEANUP( m_bullets );
		SAFE_DELETE( m_collisionManager );
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			SAFE_DELETE( m_players[i] );
			SAFE_DELETE( m_spaceships[i] );
		}
		m_socket->Close();
		SAFE_DELETE( m_socket );
		SAFE_DELETE( m_networkPacket );
	}
	//----------------------------------------------------------------------
	void ServerApp::SpawnBulletEvt( NamedProperties& params )
	{
		vec3f pos;
		float rotation;
		short teamNumber = 0;
		if( params.get( "pos", pos ) == NamedProperties::SUCCESS )
		{
			if( params.get( "rotation", rotation ) == NamedProperties::SUCCESS )
			{
				params.get( "teamNumber", teamNumber );
				std::string bulletID = "";
				if( m_spaceships[ teamNumber ] != nullptr )
					bulletID = m_spaceships[ teamNumber ]->getName();
				Bullet* bullet = new Bullet( bulletID, vec2f( pos.x, pos.y ), rotation, teamNumber, m_screenWidth, m_screenHeight, true );
				m_bullets.push_back( bullet );
				m_bulletsSpawnedThisFrame.push_back( bullet );
				//consolePrintColorf( "Spawning bullet", color::DARK_GREEN );
			}
		}
	}
	//----------------------------------------------------------------------
	void ServerApp::updateSimulation()
	{
		if( !m_gameStarted )
			m_gameStarted = ListenForPlayersToJoin();
		if( m_gameStarted )
		{
			ListenForPlayersResponse();
			UpdatePlayers();
			UpdateGameActors();
			GameApp::updateSimulation();
			m_collisionManager->udpate( DELTA_TIME );
			CheckForDeadActors();
			SendPlayerUpdate();
			m_bulletsSpawnedThisFrame.clear();
		}
	}
	//----------------------------------------------------------------------
	void ServerApp::updateDisplay()
	{}
	//----------------------------------------------------------------------
	bool ServerApp::ListenForPlayersToJoin()
	{		
		bool timeToStartGame = false;
		int packetID = -1;
		PacketType type = SERVER_PACKET_INVALID;
		
		if( m_socket != nullptr )
		{
			if( m_socket->Recieve( m_networkPacket, SERVER_WAIT_TIME_PER_FRAME ) > 0 )
			{
				if( m_networkPacket->GetNextChunkOfData( packetID ) )
				{
					if( m_networkPacket->GetNextChunkOfData( type ) )
					{
						switch( type )
						{
						case SERVER_PACKET_JOIN:
							AddPlayer( m_socket, packetID );
							m_socket = new SocketUDP( *m_socket );
							timeToStartGame = true;
							break;
						case SERVER_PACKET_START:
							timeToStartGame = true;
							break;
						case SERVER_PACKET_ACK:
							ProcessACK();
							break;
						}
					}
				}
			}
		}
		m_networkPacket->ResetToBeginning();
		return timeToStartGame;
	}
	//----------------------------------------------------------------------
	void ServerApp::SendPlayerUpdate()
	{
		// adding space for the packet id for each player. when the packet is sent to the player this is updated
		m_networkPacket->AddData( -1 );
		m_networkPacket->AddData( CLIENT_PACKET_UPDATE );
		
		for( int s = 0; s < MAX_NUM_PLAYERS; ++s )
		{
			if( m_players[s] == nullptr || m_spaceships[s] == nullptr )
			{
				m_networkPacket->AddData( (int)-1 );	//Ship is dead
			}
			else if( m_spaceships[s] != nullptr && m_spaceships[s]->isDead() )
			{
				m_networkPacket->AddData( (int)-1 );	//Ship is dead
			}
			else 
			{
				std::string name = m_spaceships[s]->getName();
				m_networkPacket->AddData( (int)name.size() );
				m_networkPacket->AddBufferOfData( name.c_str(), (int)name.size() );
				m_spaceships[ s ]->AddTimelineUpdateToPacket( m_networkPacket );
			}
		}

		m_networkPacket->AddData( (int)m_bulletsSpawnedThisFrame.size() );

		for( int i = 0; i < (int)m_bulletsSpawnedThisFrame.size(); ++i )
		{
			m_networkPacket->AddData( m_bulletsSpawnedThisFrame[i]->getTeamNumber() );
			m_networkPacket->AddData( m_bulletsSpawnedThisFrame[i]->getSGARotation() );
			m_networkPacket->AddData( m_bulletsSpawnedThisFrame[i]->getPosition().xy() );
		}
		
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			if( m_players[i] != nullptr )
			{
				vLog( "Sending player %d update", i );
				m_networkPacket->ResetToBeginning();
				m_players[i]->IncrementPacketIDSent();
				m_networkPacket->AddData( m_players[i]->GetMostRecentPacketIDSent() );
				m_players[i]->SendPriorityUpdate( m_networkPacket, m_players[i]->GetMostRecentPacketIDSent() );
			}
		}
		m_networkPacket->ResetToBeginning();		
	}
	//----------------------------------------------------------------------
	void ServerApp::ListenForPlayersResponse()
	{
		int responses = 0;
		PacketType type = SERVER_PACKET_INVALID;
		int packetID = -1;
		short teamNumber = -1;
		
		//if( m_currentNumPlayers > 0 )
		{
			while( m_socket->Recieve( m_networkPacket, SERVER_WAIT_TIME_PER_FRAME ) > 0 )
			{
				m_networkPacket->GetNextChunkOfData( packetID );
				m_networkPacket->GetNextChunkOfData( type );
				switch( type )
				{
				case SERVER_PACKET_JOIN:
					AddPlayer( m_socket, packetID );
					m_socket = new SocketUDP( *m_socket );
					++responses;
					break;
				case SERVER_PACKET_UPDATE:
					ProcessPlayerPacket( packetID );
					++responses;
					break;
				case SERVER_PACKET_ACK:
					ProcessACK();
					break;
				case SERVER_PACKET_QUIT:
					m_networkPacket->GetNextChunkOfData( teamNumber );
					RemovePlayer( teamNumber );
					++responses;
					break;
				}
				m_networkPacket->ResetToBeginning();
			}
		}
		ProcessPlayerDrop();
	}
	//----------------------------------------------------------------------
	void ServerApp::UpdateGameActors()
	{
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			if( m_spaceships[i] != nullptr )
				m_spaceships[i]->update( DELTA_TIME );
		}
		std::for_each( m_bullets.begin(), m_bullets.end(),
			[&]( Bullet* bullets )
		{
			bullets->update( DELTA_TIME );
		} );
	}
	//----------------------------------------------------------------------
	void ServerApp::UpdatePlayers()
	{
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			if( m_players[i] != nullptr )
			{
				m_players[i]->update( DELTA_TIME );
				if( m_players[i]->ShouldSpawnNewShip() )
				{
					SpawnSpaceship( m_players[i]->getTeamNumber() );
				}
			}
		}	
	}
	//----------------------------------------------------------------------
	void ServerApp::AddPlayer( SocketUDP* socket, int packetID )
	{
		m_networkPacket->ResetToBeginning();
		m_networkPacket->AddData( ++packetID );
		m_networkPacket->AddData( CLIENT_PACKET_REPLY_TO_JOIN );
		int nextPlayerNum = getOpenPlayerNum();
		if( nextPlayerNum < MAX_NUM_PLAYERS && m_players[ nextPlayerNum ] == nullptr )
		{
			m_players[ nextPlayerNum ] = new NetworkPlayerController( nextPlayerNum, socket );
			m_players[ nextPlayerNum ]->SetMostRecentPacketIDRecieved( packetID );
			consolePrintColorf( "Player: %d joined game", color::GREEN, nextPlayerNum );
			socket->PrintHostLastRecievedFromToConsole();
			m_networkPacket->AddData( nextPlayerNum );
			SpawnSpaceship( nextPlayerNum );
			++m_currentNumPlayers;
			m_players[ nextPlayerNum ]->SendPriorityUpdate( m_networkPacket, packetID );
		}
		else
		{
			m_networkPacket->AddData( (short)-1 );	// -1 means we don't have any room for you
			socket->ReplyToLastRecieve( m_networkPacket );
		}
		consolePrintf( "Replying to player..." );
		
		m_networkPacket->ResetToBeginning();
	}
	//----------------------------------------------------------------------
	void ServerApp::RemovePlayer( short teamNumber )
	{
		if( 0 <= teamNumber && teamNumber < MAX_NUM_PLAYERS )
		{
			if( m_players[ teamNumber ] != nullptr )
			{
				consolePrintf( "Removing player: %d from game", teamNumber );
				m_networkPacket->AddData( CLIENT_PACKET_DISCONNECTED );
				m_networkPacket->AddData( teamNumber );
				m_players[ teamNumber ]->SendUpdate( m_networkPacket );
				m_networkPacket->ResetToBeginning();
				SAFE_DELETE( m_players[ teamNumber ] );
				SAFE_DELETE( m_spaceships[ teamNumber ] );
				--m_currentNumPlayers;
			}
		}
	}
	//----------------------------------------------------------------------
	void ServerApp::ProcessPlayerPacket( int packetID )
	{
		short teamNumber = -1;
		int numberOfKeys = -1;

		m_networkPacket->GetNextChunkOfData( teamNumber );
		if( teamNumber < MAX_NUM_PLAYERS && teamNumber >= 0 )
		{
			if( m_players[ teamNumber ] != nullptr )
			{
				if( packetID > m_players[ teamNumber ]->GetMostRecentPacketIDRecieved() )
				{
					m_players[ teamNumber ]->UpdateRecieved();
					m_players[ teamNumber ]->SetMostRecentPacketIDRecieved( packetID );
					if( m_spaceships[ teamNumber ] != nullptr )
					{
						SGA_StateTimeline::ProcessTimelineUpdate( m_networkPacket, "" );
					}
				}
			}
		}
		else
		{
			consolePrintColorf( "Player index out of range: %d", color::RED, teamNumber );
		}
	}
	//----------------------------------------------------------------------
	void ServerApp::ProcessPlayerDrop()
	{
		int currentNumPlayers = 0;
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			if( m_players[i] != nullptr )
			{
				if( m_players[i]->ShouldDrop() )
				{
					RemovePlayer( i );
				}
				else
					currentNumPlayers++;
			}
		}
		if( currentNumPlayers != m_currentNumPlayers )
			m_currentNumPlayers = currentNumPlayers;
	}
	//----------------------------------------------------------------------
	void ServerApp::ProcessACK()
	{
		short teamNumber = -1;
		int packetID = -1;
		m_networkPacket->GetNextChunkOfData( teamNumber );
		m_networkPacket->GetNextChunkOfData( packetID );
		if( teamNumber >= 0 && m_players[ teamNumber ] != nullptr )
		{
			m_players[ teamNumber ]->RecieveACK( packetID );
		}
		vLog( "Recieved ACK" );
	}
	//----------------------------------------------------------------------
	void ServerApp::SpawnSpaceship( short teamNumber )
	{
		if( 0 <= teamNumber && teamNumber < MAX_NUM_PLAYERS && m_players[ teamNumber ] != nullptr )
		{
			SAFE_DELETE( m_spaceships[ teamNumber ] );
			m_spaceships[ teamNumber ] = new Spaceship( GetShipName( teamNumber ), teamNumber, m_screenWidth, m_screenHeight, true );
			m_players[ teamNumber ]->assignShip( m_spaceships[ teamNumber ] );
		}
		if( m_spaceships[ teamNumber ] != nullptr )
		{
			m_spaceships[ teamNumber ]->setPosition( vec2f( 50.0f, 50.0f ) );
		}
	}
	//----------------------------------------------------------------------
	std::string& ServerApp::GetShipName( short teamNumber )
	{
		static bool isInitialized = false;
		static std::vector<std::string> namePool;

		if( !isInitialized )
		{
			namePool.push_back( "Enterprise" );
			namePool.push_back( "Prometheus" );
			namePool.push_back( "Daedalus" );
			namePool.push_back( "Puddle Jumper" );
			namePool.push_back( "Orion" );
			namePool.push_back( "Aurora" );
			namePool.push_back( "Odyssey" );
			namePool.push_back( "The O'neill" );
			isInitialized = true;
		}
		if( teamNumber < 0 || teamNumber >= (int)namePool.size() )
			teamNumber = namePool.size() - 1;

		return namePool[ teamNumber ];
	}
	//----------------------------------------------------------------------
	void ServerApp::CheckForDeadActors()
	{		
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			if( m_spaceships[i] != nullptr )
			{
				if( m_spaceships[i]->isDead() )
				{
					SAFE_DELETE( m_spaceships[i] );
				}
			}
		}

		m_bullets.erase( 
			std::remove_if( m_bullets.begin(), m_bullets.end(),
			[&]( Bullet* bullet ) -> bool
		{
			if( bullet->isDead() )
			{
				SAFE_DELETE( bullet );
				return true;
			}
			else
			{
				return false;
			}
		} ), m_bullets.end() );
	}
	//----------------------------------------------------------------------
	short ServerApp::getOpenPlayerNum()
	{
		short openTeamNum = m_currentNumPlayers;
		for( int i = 0; i < MAX_NUM_PLAYERS; ++i )
		{
			if( m_players[i] == nullptr )
			{
				openTeamNum = i;
				break;
			}
		}
		return openTeamNum;
	}
}