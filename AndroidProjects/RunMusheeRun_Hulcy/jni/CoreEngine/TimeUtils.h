#pragma once

/*************************
* author: Preston Hulcy
* last updated: 6-5-13
*************************/


namespace Monky
{
	class GameApp;

	class TimeUtils
	{

	public:
		friend class GameApp;

		enum Units
		{
			SECONDS,
			MILLI,
			MICRO,
		};

		static double GetAbsoluteTimeSeconds();
		static double GetAbsoluteTime( Units units );
		static void TimeWaster( double seconds );
		static double ConvertTimeFromSecondsTo( double timeSeconds, Units units );
		static double ConvertFromUnitsToSeconds( double time, Units units );

		static float GetCurrentApplicationTimeSeconds();

	private:
		static float m_currentApplicationTime;
	};
}
