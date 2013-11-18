#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 9/29/2013
//	Description:
//
//---------------------------------------------
#include "CommonEngineIncludes.h"
#include "Packet.h"
#include "Timeline.h"
#include "SocketUDP.h"
#include <ctime>

#define MAX_TIMELINE_SIZE 30

namespace Monky
{
	
	template< typename T >
	class Timeline
	{
	public:
		Timeline( const std::string& name );
		~Timeline();

		void SetSocket( SocketUDP* socket ) { m_socket = socket; }

		void Set( int timeOffsetInMS, const T& value, bool clearEverythingAfter = false );
		T Get( int timeOffsetInMS ) const;

		virtual void SendUpdate( int packetID );
		virtual void RecieveUpdate( Packet* packet, bool apply = true );

		const std::string& GetName() const { return m_name; }

	protected:
		virtual T Interpolate( const std::pair< time_t, T >& first, const std::pair< time_t, T >& second, time_t sampleAt ) const;
		virtual T Extrapolate( const std::pair< time_t, T >& first, const std::pair< time_t, T >& second, time_t sampleAt ) const;
		

	public:
		static void ProcessTimelineUpdate( Packet* packet, const std::string& timelinesToIgnoreCSV );

	protected:
		const std::string m_name;
		std::list< std::pair< time_t, T > > m_timeline;
		SocketUDP* m_socket;
		std::vector< std::pair< time_t, T > > m_pairsToSendInUpdate;

		static std::map< std::string, Timeline<T>* > sm_timelines;
		static int sm_timelineID;
	};

	template< typename T >
	std::map< std::string, Timeline<T>* > Timeline<T>::sm_timelines;
	template< typename T >
	int Timeline<T>::sm_timelineID = 0;
	//----------------------------------------------------------------------
	template< typename T >
	Timeline<T>::Timeline( const std::string& name )
		:	m_name( name )
		,	m_socket( nullptr )
	{
		Timeline<T>*& newTimline = sm_timelines[ m_name ];		
		SAFE_DELETE( newTimline );
		newTimline = this;
	}
	//----------------------------------------------------------------------
	template< typename T >
	Timeline<T>::~Timeline()
	{
		m_timeline.clear();
		sm_timelines.erase( m_name );
	}
	//----------------------------------------------------------------------
	template< typename T >
	void Timeline<T>::Set( int timeOffsetInMS, const T& value, bool clearEverythingAfter )
	{
		PROFILE_SECTION( "Timeline Set" )
		time_t currentTime = 0;
		time( &currentTime );
		currentTime *= 1000;
		time_t timeToInsertAt = currentTime + (time_t)timeOffsetInMS;
		bool didInsert = false;
		std::pair< time_t, T > valueToInsert = std::pair< time_t, T >( timeToInsertAt, value );


		auto nextIter = m_timeline.begin();
		for( auto iter = m_timeline.begin(); !didInsert && iter != m_timeline.end(); )
		{
			++nextIter;
			if( timeToInsertAt == iter->first )
			{
				iter->second = value;
				if( clearEverythingAfter )
					m_timeline.erase( ++iter, m_timeline.end() );
				didInsert = true;
			}
			else if( nextIter != m_timeline.end() && nextIter->first > timeToInsertAt && timeToInsertAt > iter->first  )
			{
				m_timeline.insert( nextIter, valueToInsert );
				if( clearEverythingAfter )
					m_timeline.erase( nextIter, m_timeline.end() );
				didInsert = true;
			}
			
			if( !didInsert )
				++iter;
		}
		if( !didInsert )
		{
			m_timeline.insert( m_timeline.end(), valueToInsert );
		}
		m_pairsToSendInUpdate.push_back( valueToInsert );
		int differenceBetweenMax = m_timeline.size() - MAX_TIMELINE_SIZE;
		if( differenceBetweenMax > 0 )
		{
			auto iterToEraseTo = m_timeline.begin();
			for( int i = 0; i < differenceBetweenMax; ++iterToEraseTo, ++i );

			m_timeline.erase( m_timeline.begin(), iterToEraseTo );
		}
	}
	//----------------------------------------------------------------------
	template< typename T >
	T Timeline<T>::Get( int timeOffsetInMS ) const
	{
		auto prevIter = m_timeline.end();
		time_t currentTime = 0;
		time( &currentTime );
		currentTime *= 1000;
		time_t timeToRetrieveAt = currentTime + (time_t)timeOffsetInMS;
		bool found = false;
		T valueToReturn;
		
		for( auto currentIter = m_timeline.begin(); currentIter != m_timeline.end() && !found; ++currentIter )
		{
			if( currentIter->first == timeToRetrieveAt )
			{
				valueToReturn = currentIter->second;
				found = true;
			}
			else if( currentIter->first >= timeToRetrieveAt )
			{
				if( prevIter != m_timeline.end() && prevIter->first <= timeToRetrieveAt )
				{
					valueToReturn = Interpolate( *prevIter, *currentIter, timeToRetrieveAt );
					found = true;
				}
			}
			prevIter = currentIter;
		}
		
		if( !found )
		{
			if( !m_timeline.empty() )
			{
				auto lastIter = m_timeline.end();
				--lastIter;
				if( lastIter != m_timeline.begin() )
				{
					auto nextToLastIter = lastIter;
					--nextToLastIter;
					valueToReturn = Extrapolate( *nextToLastIter, *lastIter, timeToRetrieveAt );
				}
				else
					valueToReturn = m_timeline.back().second;
			}
		}

		return valueToReturn;
	}
	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	template< typename T >
	void Timeline<T>::SendUpdate( int packetID )
	{
		//I'm not going to do anything for you... do it yourself lol
	}
	//----------------------------------------------------------------------
	template< typename T >
	void Timeline<T>::RecieveUpdate( Packet* packet, bool apply )
	{
		//Same here lol
	}
	//----------------------------------------------------------------------
	template< typename T >
	T Timeline<T>::Interpolate( const std::pair< time_t, T >& first, const std::pair< time_t, T >& second, time_t sampleAt ) const
	{
		float tValue = (float)( sampleAt - first.first ) / (float)( second.first - first.first );
		T value = first.second * ( 1.0f - tValue ) + second.second * tValue; 
		return value;
	}
	//----------------------------------------------------------------------
	template< typename T >
	T Timeline<T>::Extrapolate( const std::pair< time_t, T >& first, const std::pair< time_t, T >& second, time_t sampleAt ) const
	{
		return second.second;
	}
	//----------------------------------------------------------------------
	template< typename T >
	void Timeline<T>::ProcessTimelineUpdate( Packet* packet, const std::string& timelinesToIgnoreCSV )
	{ 
		std::vector< std::string > tokens;
		stringTokenizer( timelinesToIgnoreCSV, tokens, "," );
		int nameLen = 0;
		char nameBuffer[ MAX_NAME_SIZE ];
		packet->GetNextChunkOfData( nameLen );
		if( nameLen > 0 )
		{
			packet->GetBufferOfData( nameBuffer, nameLen );
			if( nameLen < MAX_NAME_SIZE )
				nameBuffer[ nameLen ] = '\0';
			else
				nameBuffer[ MAX_NAME_SIZE - 1 ] = '\0';
			std::string name = nameBuffer;

			bool skipTimelineUpdate= false;
			for( size_t i = 0; i < tokens.size(); ++i )
				if( tokens[i] == name )
					skipTimelineUpdate = true;

			auto iter = sm_timelines.find( name );
			if( iter != sm_timelines.end() )
			{
				iter->second->RecieveUpdate( packet, !skipTimelineUpdate );
			}
		}
	}
}