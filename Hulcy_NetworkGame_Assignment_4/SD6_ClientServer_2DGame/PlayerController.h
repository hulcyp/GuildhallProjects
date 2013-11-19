#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/21/2013
//	Description: Represents a player in the game that can control a spaceship
//
//---------------------------------------------
#include "InputListener.h"
#include "CommonEngineIncludes.h"
#include "Timer.h"

namespace Monky
{
	class Spaceship;

	class PlayerController
	{
	public:
		PlayerController( int teamNumber );
		virtual ~PlayerController();

		void applyClockwiseRotation();
		void applyCounterClockwiseRotation();

		void applyForwardImpulse();
		void applyReverseImpulse();

		void fireBullet();

		virtual void update( double deltaTime );

		Spaceship* getSpaceShip() const;
		virtual void assignShip( Spaceship* ship );
		
		void shipDiedEvt( NamedProperties& params );

		bool ShouldSpawnNewShip() const;

		short getTeamNumber() const { return m_teamNumber; }

	protected:
		Spaceship* m_ship;
		Timer* m_shipRespawnTimer;
		short m_teamNumber;
	};
}