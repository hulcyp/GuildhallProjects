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
	class LocalPlayerController;
	class Spaceship;
	class Bullet;
	class SocketUDP;
	class CollisionManager;
	class Packet;

	class ClientApp : public GameApp
	{
	public:
		ClientApp( double deltaTime, int width, int height, float fov, const char* windowTitle );
		virtual ~ClientApp();

		virtual void cleanup();

		bool ConnectToServer( const std::string& host, int localPort, int serverPort );
		
		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y );

	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();

		void SpawnSpaceship( const std::string& name, short teamNumber, const vec2f& pos );
		void SpawnBullet( const vec2f& pos, float rotation, short teamNumber );

		void SendUpdateToServer();
		bool RecieveUpdateFromServer();

		void HandleServerUpdate();
		void HandleDisconnect();
		
	private:
		Spaceship* m_spaceships[ MAX_NUM_PLAYERS ];
		LocalPlayerController* m_playerController;
		SocketUDP* m_serverSocket;
		Packet* m_networkPacket;
		Packet* m_ackPacket;
		CollisionManager* m_collisionManager;
		//bool m_keyboard[ MONKY_KEY_COUNT ];	
		//int m_teamNumber;
		std::string m_serverHostName;
		int m_serverPort;
		int m_mostRecentPacketIDRecieved;
		int m_mostRecentPacketIDSent;
	};
}