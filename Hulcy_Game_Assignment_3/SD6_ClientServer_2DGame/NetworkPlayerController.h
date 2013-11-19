#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/31/2013
//	Description:
//
//---------------------------------------------
#include "PlayerController.h"

namespace Monky
{
	class SocketUDP;
	class Packet;
	class Timer;

	class NetworkPlayerController : public PlayerController
	{
	public:
		NetworkPlayerController( int teamNumber, SocketUDP* socketToSendTo );
		virtual ~NetworkPlayerController();

		bool onKeyDown( MonkyKey keyCode, int x, int y );
		void clearKeyPresses();

		virtual void update( double deltaTime );

		void SendUpdate( char* buffer, int bufferSize );
		void SendUpdate( Packet* packet );
		void SendPriorityUpdate( Packet* packet, int idx );
		void UpdateRecieved();
		bool ShouldDrop() const;

		void SetMostRecentPacketIDRecieved( int id );
		int GetMostRecentPacketIDRecieved() const;
		int GetMostRecentPacketIDSent() const;
		void IncrementPacketIDSent();

		void RecieveACK( int packetIdx );

	private:
		std::map< int, Packet* > m_packetsWaitingToBeACKd;
		bool m_keyboard[ MONKY_KEY_COUNT ];
		SocketUDP* m_socketToSendTo;
		Timer* m_dropTimer;
		int m_mostRecentPacketIDSent;
		int m_mostRecentPacketIDRecieved;
	};
}