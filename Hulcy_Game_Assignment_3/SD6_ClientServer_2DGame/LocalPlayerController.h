#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/21/2013
//	Description:
//
//---------------------------------------------
#include "InputListener.h"
#include "PlayerController.h"

namespace Monky
{
	class LocalPlayerController : public InputListener, public PlayerController
	{
	public:
		LocalPlayerController( int teamNumber );
		virtual ~LocalPlayerController();

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y );

		virtual void update( double deltaTime );

		std::vector< MonkyKey > GetKeysPressed() const;

	private:
		bool m_keyboard[ MONKY_KEY_COUNT ];
	};
}