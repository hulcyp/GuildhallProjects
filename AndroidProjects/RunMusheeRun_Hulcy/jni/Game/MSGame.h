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

		virtual void reloadRenderingAssets();

	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();



	private:

		TiledMap* m_tiledMap;
		vec2f m_lastPosDown;
		SpriteActor* m_theMushee;

		vec2f m_currentCheckpoint;

		float m_currentHorizontalImpulseToApply;
		float m_verticalImpulseToApply;
	};
}
