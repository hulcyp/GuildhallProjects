#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/20/2013
//	Description:
//
//---------------------------------------------
#include <GameApp.h>

namespace Monky
{
	class Spaceship;
	class LocalPlayerController;
	class CollisionManager;

	class App : public GameApp
	{
	public:
		App( double deltaTime, int width, int height, float fov, const char* windowTitle );
		virtual ~App();

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );

		void spawnBulletEvt( NamedProperties& params );

	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();

		virtual void renderDebugHUD();

	private:
		Spaceship* m_ship;
		LocalPlayerController* m_localPlayer;
		CollisionManager* m_collisionManager;
	};
}