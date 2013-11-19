#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 9/9/2013
//	Description:
//
//---------------------------------------------

namespace Monky
{
	class Timer;
	class Packet
	{
	public:
		Packet( int packetSize, int resendTimerMS = 500 );
		Packet( Packet& copy );
		~Packet();

		char* GetBuffer() const;
		int GetBufferSize() const;
		void ResetToBeginning();
		bool AddBufferOfData( const char* buffer, int size );
		bool GetBufferOfData( char* buffer, int size );

		void SetResendTimer( int ms );
		void StartResendTimer();
		bool ShouldResend() const;

		template< typename T >
		bool AddData( const T& data )
		{
			bool succeeded = false;
			int sizeOfData = sizeof( T );

			if( m_nextAddrToAddData + sizeOfData < m_buffer + m_bufferSize )
			{
				*reinterpret_cast<T*>( m_nextAddrToAddData ) = data;
				m_nextAddrToAddData += sizeof( T );
				succeeded = true;
			}			
			return succeeded;
		}
		//----------------------------------------------------------------------
		template< typename T >
		bool GetNextChunkOfData( T& outData )
		{
			bool succeeded = false;
			int sizeOfData = sizeof( T );

			if( m_nextAddrToAddData + sizeOfData <= m_buffer + m_bufferSize )
			{
				outData = *reinterpret_cast< T* >( m_nextAddrToAddData );
				m_nextAddrToAddData += sizeOfData;
				succeeded = true;
			}
			return succeeded;
		}

	private:
		char* m_buffer;
		char* m_nextAddrToAddData;
		Timer* m_shouldResendTimer;
		int m_bufferSize;
	};
}