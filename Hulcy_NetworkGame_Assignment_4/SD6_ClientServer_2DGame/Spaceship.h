#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/21/2013
//	Description: Represents a spaceship in the game
//
//---------------------------------------------
#include "SpaceGameActor.h"


namespace Monky
{
	class PlayerController;
	class Timer;

	class Spaceship : public SpaceGameActor
	{
	public:
		Spaceship( const std::string& spaceshipName, short teamNumber, int screenWidth, int screenHeight, bool isServerSide = false );
		virtual ~Spaceship();

		virtual void render();
		virtual void update( double deltaTime );

		void fireBullet();
		void assignTeamNumber( short teamNumber );
		const std::string& getName() const { return m_spaceShipName; }

	protected:
		void createMesh( const Color4f& color );

	private:
		PlayerController* m_owner;	
		Timer* m_weaponTimer;
		const std::string m_spaceShipName;
		NamedProperties m_fontParams;
	};

	Color4f getTeamColor( short teamNumber );
}