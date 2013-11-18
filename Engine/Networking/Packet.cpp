#include "Packet.h"
#include "CommonEngineIncludes.h"
#include "Timer.h"
#include "TimeUtils.h"

namespace Monky
{
	Packet::Packet( int packetSize, int resendTimerMS  )
		:	m_bufferSize( packetSize )
		,	m_buffer( nullptr )
		,	m_nextAddrToAddData( nullptr )
		,	m_shouldResendTimer( nullptr )
	{
		if( m_bufferSize > 0 )
		{
			m_buffer = new char[ packetSize ];
			m_nextAddrToAddData = m_buffer;
		}
		m_shouldResendTimer = new Timer( TimeUtils::ConvertFromUnitsToSeconds( (double)resendTimerMS, TimeUtils::MILLI ) );
	}
	//----------------------------------------------------------------------
	Packet::Packet( Packet& copy )
	{
		m_bufferSize = copy.m_bufferSize;
		m_buffer = new char[ m_bufferSize ];
		std::memcpy( m_buffer, copy.m_buffer, m_bufferSize );
		m_nextAddrToAddData = m_buffer + ( copy.m_nextAddrToAddData - copy.m_buffer );
	}
	//----------------------------------------------------------------------
	Packet::~Packet()
	{
		SAFE_DELETE( m_buffer );
		m_nextAddrToAddData = nullptr;
		m_bufferSize = 0;
	}
	//----------------------------------------------------------------------
	char* Packet::GetBuffer() const
	{
		return m_buffer;
	}
	//----------------------------------------------------------------------
	int Packet::GetBufferSize() const
	{
		return m_bufferSize;
	}
	//----------------------------------------------------------------------
	void Packet::ResetToBeginning()
	{
		m_nextAddrToAddData = m_buffer;
	}
	//----------------------------------------------------------------------
	bool Packet::AddBufferOfData( const char* buffer, int size )
	{
		bool succeeded = false;

		if( m_nextAddrToAddData + size < m_buffer + m_bufferSize )
		{
			std::memcpy( m_nextAddrToAddData, buffer, size );
			succeeded = true;
			m_nextAddrToAddData += size;
		}
		return succeeded;
	}
	//----------------------------------------------------------------------
	bool Packet::GetBufferOfData( char* buffer, int size )
	{
		bool succeeded = false;

		if( m_nextAddrToAddData + size < m_buffer + m_bufferSize )
		{
			std::memcpy( buffer, m_nextAddrToAddData, size );
			succeeded = true;
			m_nextAddrToAddData += size;
		}
		return succeeded;
	}
	//----------------------------------------------------------------------
	void Packet::SetResendTimer( int ms )
	{
		m_shouldResendTimer->setTime( TimeUtils::ConvertFromUnitsToSeconds( ms, TimeUtils::MILLI ) );
	}
	//----------------------------------------------------------------------
	void Packet::StartResendTimer()
	{
		m_shouldResendTimer->start();
	}
	//----------------------------------------------------------------------
	bool Packet::ShouldResend() const
	{
		return m_shouldResendTimer->isTimeUp();
	}
}