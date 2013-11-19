#include "NetworkPlayerController.h"
#include "SocketUDP.h"
#include "ClientServerInterface.h"
#include "Packet.h"
#include "Spaceship.h"

namespace Monky
{
	NetworkPlayerController::NetworkPlayerController( int teamNumber, SocketUDP* socketToSendTo )
		:	PlayerController( teamNumber )
		,	m_socketToSendTo( socketToSendTo )
		,	m_mostRecentPacketIDRecieved( 0 )
		,	m_mostRecentPacketIDSent( 0 )
	{
		m_dropTimer = new Timer( TimeUtils::ConvertTimeFromSecondsTo( WAIT_TIME_BEFORE_SERVER_DROPS_CLIENT, TimeUtils::MILLI ) );
		clearKeyPresses();
	}
	//----------------------------------------------------------------------
	NetworkPlayerController::~NetworkPlayerController()
	{
		MAP_CLEANUP( m_packetsWaitingToBeACKd );
		SAFE_DELETE( m_dropTimer );
		//m_socketToSendTo->Close();
		SAFE_DELETE( m_socketToSendTo );
	}
	//----------------------------------------------------------------------
	bool NetworkPlayerController::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		m_keyboard[ keyCode ] = true;
		return true;
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::clearKeyPresses()
	{
		for( int i = 0; i < MONKY_KEY_COUNT; ++i )
		{
			m_keyboard[i] = false;
		}
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::update( double deltaTime )
	{
		if( m_ship != nullptr )
		{
			if( m_keyboard[ MONKY_UP ] || m_keyboard[ MONKY_w ] )
				applyForwardImpulse();
			if( m_keyboard[ MONKY_DOWN ] || m_keyboard[ MONKY_s ] )
				applyReverseImpulse();
			if( m_keyboard[ MONKY_LEFT ]  || m_keyboard[ MONKY_a ] )
				applyCounterClockwiseRotation();
			if( m_keyboard[ MONKY_RIGHT ]  || m_keyboard[ MONKY_d ] )
				applyClockwiseRotation();
			if( m_keyboard[ MONKY_SPACE ] )
				fireBullet();
		}
		clearKeyPresses();

		for( auto iter = m_packetsWaitingToBeACKd.begin(); iter != m_packetsWaitingToBeACKd.end(); ++iter )
		{
			if( iter->second->ShouldResend() )
			{
				SendUpdate( iter->second );
				iter->second->StartResendTimer();
			}
		}
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::assignShip( Spaceship* ship )
	{
		PlayerController::assignShip( ship );
		if( ship != nullptr )
			ship->SetSocketForTimeline( m_socketToSendTo );
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::SendUpdate( char* buffer, int bufferSize )
	{
		m_socketToSendTo->ReplyToLastRecieve( buffer, bufferSize );
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::SendUpdate( Packet* packet )
	{
		m_socketToSendTo->ReplyToLastRecieve( packet );
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::SendPriorityUpdate( Packet* packet, int idx )
	{
		m_socketToSendTo->ReplyToLastRecieve( packet );
		packet->StartResendTimer();
		auto iter = m_packetsWaitingToBeACKd.find( idx );
		if( iter != m_packetsWaitingToBeACKd.end() )
		{
			SAFE_DELETE( iter->second );
			iter->second = new Packet( PACKET_SIZE );
		}
		else
		{
			m_packetsWaitingToBeACKd.insert( std::pair< int, Packet* >( idx, new Packet( PACKET_SIZE ) ) );
		}
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::UpdateRecieved()
	{
		m_dropTimer->start();
	}
	//----------------------------------------------------------------------
	bool NetworkPlayerController::ShouldDrop() const
	{
		return m_dropTimer->isTimeUp();
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::SetMostRecentPacketIDRecieved( int id )
	{
		m_mostRecentPacketIDRecieved = id;
	}
	//----------------------------------------------------------------------
	int NetworkPlayerController::GetMostRecentPacketIDRecieved() const
	{
		return m_mostRecentPacketIDRecieved;
	}
	int NetworkPlayerController::GetMostRecentPacketIDSent() const
	{
		return m_mostRecentPacketIDSent;
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::IncrementPacketIDSent()
	{
		++m_mostRecentPacketIDSent;
	}
	//----------------------------------------------------------------------
	void NetworkPlayerController::RecieveACK( int packetIdx )
	{
		auto iter = m_packetsWaitingToBeACKd.find( packetIdx );
		if( iter != m_packetsWaitingToBeACKd.end() )
		{
			SAFE_DELETE( iter->second );
			m_packetsWaitingToBeACKd.erase( iter );
		}
	}
}