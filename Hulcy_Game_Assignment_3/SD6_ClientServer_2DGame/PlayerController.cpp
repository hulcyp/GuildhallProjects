#include "PlayerController.h"
#include "Spaceship.h"

namespace Monky
{
	PlayerController::PlayerController( int teamNumber )
		:	m_ship( nullptr )
		,	m_teamNumber( teamNumber )
	{
		registerForEvent( "SpaceshipDied", *this, &PlayerController::shipDiedEvt );
		m_shipRespawnTimer = new Timer( 3.0f );
	}
	//----------------------------------------------------------------------
	PlayerController::~PlayerController()
	{
		m_ship = nullptr;
		unregisterForEvent( *this );
	}
	//----------------------------------------------------------------------
	void PlayerController::applyClockwiseRotation()
	{
		if( m_ship != nullptr )
			m_ship->applyClockwiseRotation();
	}
	//----------------------------------------------------------------------
	void PlayerController::applyCounterClockwiseRotation()
	{
		if( m_ship != nullptr )
			m_ship->applyCounterClockwiseRotation();
	}
	//----------------------------------------------------------------------
	void PlayerController::applyForwardImpulse()
	{
		if( m_ship != nullptr )
			m_ship->applyForwardImpulse();
	}
	//----------------------------------------------------------------------
	void PlayerController::applyReverseImpulse()
	{
		if( m_ship != nullptr )
			m_ship->applyReverseImpulse();
	}
	//----------------------------------------------------------------------
	void PlayerController::fireBullet()
	{
		if( m_ship != nullptr )
			m_ship->fireBullet();
	}
	//----------------------------------------------------------------------
	void PlayerController::update( double deltaTime )
	{}
	//----------------------------------------------------------------------
	Spaceship* PlayerController::getSpaceShip() const
	{
		return m_ship;
	}
	//----------------------------------------------------------------------
	void PlayerController::assignShip( Spaceship* ship )
	{
		m_ship = ship;
		if( m_ship != nullptr )
			m_ship->assignTeamNumber( m_teamNumber );
	}
	//----------------------------------------------------------------------
	void PlayerController::shipDiedEvt( NamedProperties& params )
	{
		Spaceship* tempShip;
		if( params.get( "spaceship", tempShip ) == NamedProperties::SUCCESS )
		{
			if( tempShip == m_ship )
			{
				m_shipRespawnTimer->start();
				m_ship = nullptr;
			}
		}
	}
	//----------------------------------------------------------------------
	bool PlayerController::ShouldSpawnNewShip() const
	{
		return m_shipRespawnTimer->isTimeUp();
	}
}