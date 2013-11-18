#ifndef Monky_CAMERA
#define Monky_CAMERA

#include "Camera.h"
#include "InputListener.h"

namespace Monky
{

	class MCCamera : public Camera, public InputListener
	{
	public:
		MCCamera( const std::string& id, const vec3f& pos, const vec2i& mousePos, const mat4f& projMatrix, const mat4f& viewMatrix = mat4f::IDENTITY );
		MCCamera& operator=( const MCCamera& rhs );
		~MCCamera();

#pragma warning( disable:4100 )

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y );		
		virtual bool onMouseButton( int mouseKeyCode, MonkyMouseButtonState state, int x, int y ){ return false; }
		virtual void onMouseMove( int x, int y );
#pragma warning( default:4100 )

		virtual void updateInput();
		virtual void update( double deltaTime );
		
		void setBoost( bool boost ){ m_boost = boost; }
		virtual const mat4f& getViewMatrix() { return m_viewMatrix; }
		const vec3f& getVelocity() const { return m_vel; }

	private:
		const float MOUSE_YAW_POWER;
		const float MOUSE_PITCH_POWER;
		const float KEYBOARD_IMPULSE_POWER;
		const float BOOST_FACTOR;
		vec3f m_impulse;
		bool m_boost;
		vec3f m_rot;
		vec3f m_vel;
		float m_drag;
		
		vec2i m_mouseCenterPos;
		bool m_mouseInputUpdated;

		bool m_keyboard[ 256 ];
	};
}

#endif