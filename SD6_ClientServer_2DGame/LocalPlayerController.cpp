#include "LocalPlayerController.h"

namespace Monky
{
	LocalPlayerController::LocalPlayerController( int teamNumber )
		:	PlayerController( teamNumber )
	{
		for( int i = 0; i < MONKY_KEY_COUNT; ++i )
		{
			m_keyboard[i] = false;
		}
	}
	//----------------------------------------------------------------------
	LocalPlayerController::~LocalPlayerController()
	{}
	//----------------------------------------------------------------------
	bool LocalPlayerController::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		m_keyboard[ keyCode ] = true;
		return true;
	}
	//----------------------------------------------------------------------
	bool LocalPlayerController::onKeyUp( MonkyKey keyCode, int x, int y )
	{
		m_keyboard[ keyCode ] = false;
		return true;
	}
	//----------------------------------------------------------------------
	void LocalPlayerController::update( double deltaTime )
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
	}
	//----------------------------------------------------------------------
	std::vector< MonkyKey > LocalPlayerController::GetKeysPressed() const
	{
		std::vector< MonkyKey > keysPressed;
		if( m_keyboard[ MONKY_w ] )
			keysPressed.push_back( MONKY_w );
		if( m_keyboard[ MONKY_a ] )
			keysPressed.push_back( MONKY_a );
		if( m_keyboard[ MONKY_s ] )
			keysPressed.push_back( MONKY_s );
		if( m_keyboard[ MONKY_d ] )
			keysPressed.push_back( MONKY_d );
		if( m_keyboard[ MONKY_SPACE ] )
			keysPressed.push_back( MONKY_SPACE );
		
		return keysPressed;
	}
}