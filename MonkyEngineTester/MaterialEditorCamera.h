#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/23/2014
//	Description:
//
//---------------------------------------------
#include "Camera.h"
#include "InputListener.h"

namespace Monky
{
	class MaterialEditorCamera : public Camera, public InputListener
	{
	public:
		MaterialEditorCamera( const std::string& id, const vec3f& pos, const vec3f& lookAt, const mat4f& projMatrix, const mat4f& viewMatrix = mat4f::IDENTITY );

		~MaterialEditorCamera();

#pragma warning( disable:4100 )

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y );		
		virtual bool onMouseButton( int mouseKeyCode, MonkyMouseButtonState state, int x, int y ){ return false; }
		virtual void onMouseMove( int x, int y );
#pragma warning( default:4100 )

		virtual void updateInput();
		virtual void update( double deltaTime );


	private:
		vec3f m_lookAtPos;

		bool m_keyboard[ 256 ];

	};
}