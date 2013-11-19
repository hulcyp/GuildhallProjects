#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/31/2013
//	Description:
//  
//---------------------------------------------
#include "GameApp.h"
#include "ClientServerInterface.h"

namespace Monky
{
	class Bullet;
	class Spaceship;
	class NetworkPlayerController;
	class SocketUDP;
	class CollisionManager;
	class Packet;

	class ServerApp : public GameApp
	{
	public:
		ServerApp( const std::string& serverAddr, int serverPort, double deltaTime, int width, int height, float fov, const char* windowTitle );
		virtual ~ServerApp();

		void SpawnBulletEvt( NamedProperties& params );

	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();

		bool ListenForPlayersToJoin();
		void SendPlayerUpdate();
		void ListenForPlayersResponse();
		void UpdateGameActors();
		void UpdatePlayers();

		void AddPlayer( SocketUDP* socket, int packetID );
		void RemovePlayer( short teamNumber );
		void ProcessPlayerPacket( int packetID );
		void ProcessPlayerDrop();

		void ProcessACK();

		void SpawnSpaceship( short teamNumber );

		std::string& GetShipName( short teamNumber );		

		void CheckForDeadActors();
		short getOpenPlayerNum();

		void clearShipForcesBitFlags();

	private:
		NetworkPlayerController* m_players[ MAX_NUM_PLAYERS ];
		Spaceship* m_spaceships[ MAX_NUM_PLAYERS ];
		std::list< Bullet* > m_bullets;
		CollisionManager* m_collisionManager;

		std::vector< Bullet* > m_bulletsSpawnedThisFrame;
		SocketUDP* m_socket;
		Packet* m_networkPacket;
		bool m_gameStarted;
		int m_currentNumPlayers;
	};
}