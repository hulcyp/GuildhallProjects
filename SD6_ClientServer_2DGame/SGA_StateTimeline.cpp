#include "SGA_StateTimeline.h"
#include "ClientServerInterface.h"
#include "SocketUDP.h"

namespace Monky
{
	SGA_State::SGA_State()
		:	rot( 0.0f )
		,	spawnBullet( false )
	{}
	//----------------------------------------------------------------------
	SGA_State SGA_State::operator*( float value ) const
	{
		SGA_State temp;
		temp.pos = pos * value;
		temp.vel = vel * value;
		temp.dir = dir * value;
		temp.rot = rot * value;
		return temp;
	}
	//----------------------------------------------------------------------
	const SGA_State& SGA_State::operator=( const SGA_State& copy )
	{
		pos = copy.pos;
		vel = copy.vel;
		dir = copy.dir;
		rot = copy.rot;
		spawnBullet = copy.spawnBullet;
		return *this;
	}
	//----------------------------------------------------------------------
	SGA_State SGA_State::operator+( const SGA_State& rhs ) const
	{
		SGA_State temp;
		temp.pos = pos + rhs.pos;
		temp.vel = vel + rhs.vel;
		temp.dir = dir + rhs.dir;
		temp.rot = rot + rhs.rot;
		return temp;
	}
	//----------------------------------------------------------------------
	SGA_StateTimeline::SGA_StateTimeline( const std::string& name, float drag )
		:	Timeline( name )
		,	m_drag( drag )
	{
		m_packet = new Packet( PACKET_SIZE );
	}
	//----------------------------------------------------------------------
	SGA_StateTimeline::~SGA_StateTimeline()
	{
		SAFE_DELETE( m_packet );
	}
	//----------------------------------------------------------------------
	void SGA_StateTimeline::SendUpdate( int packetID )
	{
		if( m_socket != nullptr )
		{
			if( !m_pairsToSendInUpdate.empty() )
			{
				vLog( "Sending timeline update for: %s", m_name.c_str() );
				m_packet->ResetToBeginning();
				m_packet->AddData( packetID );
				m_packet->AddData( TIMELINE_UPDATE );
				m_packet->AddData( (int)m_name.size() );
				m_packet->AddBufferOfData( m_name.c_str(), m_name.size() );
				m_packet->AddData( (int)m_pairsToSendInUpdate.size() );
				for( int i = 0; i < (int)m_pairsToSendInUpdate.size(); ++i )
				{
					m_packet->AddData( m_pairsToSendInUpdate[ i ] );
				}
				m_socket->ReplyToLastRecieve( m_packet );
				m_pairsToSendInUpdate.clear();
			}
		}
	}
	//----------------------------------------------------------------------
	void SGA_StateTimeline::RecieveUpdate( Packet* packet, bool apply )
	{
		time_t currentTime = 0;
		time( &currentTime );
		currentTime *= 1000;

		int numPairsToUpdate = 0;
		packet->GetNextChunkOfData( numPairsToUpdate );
		vLog( "Recieving update for: %s\n\tWith %d timeline updates", m_name.c_str(), numPairsToUpdate );
		for( int i = 0; i < numPairsToUpdate; ++i )
		{
			std::pair< time_t, SGA_State > state;
			packet->GetNextChunkOfData( state );
			if( apply )
				Set( (int)(state.first - currentTime), state.second, true );
		}
	}
	//----------------------------------------------------------------------
	void SGA_StateTimeline::AddUpdateToPacket( Packet* packet )
	{
		vLog( "Adding timeline data to update for: %s", m_name.c_str() );
		packet->AddData( (int)m_name.size() );
		packet->AddBufferOfData( m_name.c_str(), (int)m_name.size() );
		packet->AddData( (int)m_pairsToSendInUpdate.size() );
		for( int i = 0; i < (int)m_pairsToSendInUpdate.size(); ++i )
		{
			packet->AddData( m_pairsToSendInUpdate[ i ] );
		}
		m_pairsToSendInUpdate.clear();
	}
	//----------------------------------------------------------------------
	Monky::SGA_State SGA_StateTimeline::Interpolate( const std::pair< time_t, SGA_State >& first, const std::pair< time_t, SGA_State >& second, time_t sampleAt ) const
	{
		SGA_State interp;
		float delta = (float)( second.first - first.first );
		float normalizedTimeToSampleAt = (float)( sampleAt - first.first ) / delta;
		//normalizedTimeToSampleAt /= 1000.0f;

		interp = first.second  * ( 1 - normalizedTimeToSampleAt ) + second.second * normalizedTimeToSampleAt;
		//interp.spawnBullet = ( first.second.spawnBullet  && second.second.spawnBullet );
		if( normalizedTimeToSampleAt < 0.5f )
			interp.spawnBullet = first.second.spawnBullet;
		else
			interp.spawnBullet = second.second.spawnBullet;
		return interp;
	}
	//----------------------------------------------------------------------
	Monky::SGA_State SGA_StateTimeline::Extrapolate( const std::pair< time_t, SGA_State >& first, const std::pair< time_t, SGA_State >& second, time_t sampleAt ) const
	{
		SGA_State extrap;
		vec2f acceleration;
		float rotAcceleration;
		
		float timeSinceEndSeconds = (float)( sampleAt - second.first );
		float delta = (float)( second.first - first.first );
		delta /= 1000.0f;
		if( delta > 0.0f && timeSinceEndSeconds > 0.0f )
		{
			timeSinceEndSeconds /= 1000.0f;

			acceleration = ( second.second.vel - first.second.vel ) / delta;
			rotAcceleration = ( second.second.rot - first.second.rot ) / delta;


			extrap.vel = second.second.vel + acceleration * timeSinceEndSeconds;
			extrap.vel -= extrap.vel * m_drag * timeSinceEndSeconds;
			extrap.pos = second.second.pos + extrap.vel * timeSinceEndSeconds;
			extrap.rot = second.second.rot + rotAcceleration * timeSinceEndSeconds;


			mat3f rotMatrix = mat3f::createMatrixFromQuaternion( quatf::makeQuaternionFromAxisAngle( MathFuncs<float>::degreesToRadians( extrap.rot ), vec3f( 0.0f, 0.0f, 1.0f ) ) );
			vec3f tempDir = transformVector( vec3f( 0.0f, 1.0f, 0.0f ), rotMatrix );
			extrap.dir = vec2f( tempDir.x, tempDir.y );

			//consolePrintf( "Time in seconds since the end %f", timeSinceEndSeconds );
		}
		else 
			extrap = second.second;
		extrap = second.second;
		if( timeSinceEndSeconds < 0.1f )
		{
			extrap.spawnBullet = second.second.spawnBullet;
		}
		else
			extrap.spawnBullet = false;
		return extrap;
	}
}