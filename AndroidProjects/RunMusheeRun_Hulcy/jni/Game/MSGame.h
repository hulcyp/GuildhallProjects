#include "GameApp.h"

namespace Monky
{

	class SpriteActor;
	class MSGame : public GameApp
	{

	public:
		MSGame( double deltaTime, int width, int height, float fov );
		virtual ~MSGame();

		virtual void initialize();

		virtual bool onFingerDown( int fingerID, float x, float y );
		virtual bool onFingerUp( int fingerID, float x, float y );


	protected:
		virtual void updateSimulation();



	private:
		SpriteActor* m_spinningCube;

		vec2f m_lastPosDown;
	};
}
