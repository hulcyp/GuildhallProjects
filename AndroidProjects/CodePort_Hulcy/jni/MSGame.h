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

	protected:
		virtual void updateSimulation();



	private:
		SpriteActor* m_spinningCube;
	};
}
