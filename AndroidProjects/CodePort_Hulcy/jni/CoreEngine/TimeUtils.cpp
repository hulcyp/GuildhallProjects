
/*************************
* author: Preston Hulcy
* last updated: 11-14-13
*************************/
#include "TimeUtils.h"
#include <stdint.h>
#include <time.h>


namespace Monky
{
	typedef int64_t SystemClocks;

	float TimeUtils::m_currentApplicationTime = 0.0f;

	double TimeUtils::GetAbsoluteTimeSeconds()
	{
		timespec lTimeVal;
		clock_gettime( CLOCK_MONOTONIC, &lTimeVal );


		return lTimeVal.tv_sec + (lTimeVal.tv_nsec * 1.0e-9);
	}
	//---------------------------------------------------------------
	double TimeUtils::GetAbsoluteTime( Units units )
	{
		return ConvertTimeFromSecondsTo( GetAbsoluteTimeSeconds(), units );
	}
	//---------------------------------------------------------------
	void TimeUtils::TimeWaster( double seconds )
	{
		//SystemClocks now = GetAbsoluteTimeClocks();
		//SystemClocks clocksToWasteTo = now + SecondsToClocks( seconds );
		double now = GetAbsoluteTimeSeconds();
		double secondsToWaste = now + seconds;
		
		while( now < secondsToWaste )
		{
			now = GetAbsoluteTimeSeconds();
		}
	}
	//---------------------------------------------------------------
	double TimeUtils::ConvertTimeFromSecondsTo( double timeSeconds, Units units )
	{
		switch( units )
		{
		case SECONDS:
			return timeSeconds;
		case MILLI:
			return timeSeconds * 1000.0;
		case MICRO:
			return timeSeconds * 1000000.0;
		default:
			return timeSeconds;
		}
	}
	//---------------------------------------------------------------
	double TimeUtils::ConvertFromUnitsToSeconds( double time, Units units )
	{
		switch( units )
		{
		case SECONDS:
			return time;
		case MILLI:
			return time * 0.001;
		case MICRO:
			return time * 0.000001;
		default:
			return time;
		}
	}
	//---------------------------------------------------------------
	float TimeUtils::GetCurrentApplicationTimeSeconds()
	{
		return m_currentApplicationTime;
	}
}
