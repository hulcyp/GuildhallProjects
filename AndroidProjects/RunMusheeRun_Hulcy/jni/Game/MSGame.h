#include "GameApp.h"
#include "MSMap.h"

namespace Monky
{

	class SpriteActor;
	class Button;
	class MSGame : public GameApp
	{

	public:


		MSGame( double deltaTime, int width, int height, float fov );
		virtual ~MSGame();

		virtual void initialize();

		virtual bool onFingerDown( int fingerID, float x, float y );
		virtual bool onFingerUp( int fingerID, float x, float y );

		virtual void reloadRenderingAssets();


		void MoveMusheeUp( NamedProperties& params );
		void MoveMusheeDown( NamedProperties& params );
		void MusheeJump( NamedProperties& params );
		void PauseGame( NamedProperties& params );

	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();

		virtual void renderHUD();

		void SpawnMushee();
		void ClampMusheeBasedOnLayer();


	private:

		MSMap* m_randomMap;
		vec2f m_lastPosDown;
		SpriteActor* m_theMushee;
		Camera* m_hudCamera;
		Mesh* m_bgMesh;
		vec2f m_bgPosition;

		std::vector< Button* > m_buttons;

		vec2f m_jumpImpulse;
		vec2f m_gravity;
		float m_cameraOffset;
		int m_currentPlayerLayer;

		float m_currentHorizontalImpulseToApply;
		float m_verticalImpulseToApply;

		float m_respawnTimer;
		float m_respawnTime;

		bool m_isPaused;

		NamedProperties m_bgParams;
		NamedProperties m_gameMessageTextParams;
	};
}
