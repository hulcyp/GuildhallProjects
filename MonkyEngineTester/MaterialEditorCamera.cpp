#include "MaterialEditorCamera.h"

namespace Monky
{

	MaterialEditorCamera::MaterialEditorCamera( const std::string& id, const vec3f& pos, const vec3f& lookAt, const mat4f& projMatrix )
		:	Camera( id, pos, PH_PERSPECTIVE, projMatrix, mat4f::lookAt( pos, lookAt ).inverse() )
		,	InputListener( true )
		,	m_lookAtPos( lookAt )
		,	m_rightButtonDown( false )
		,   m_leftButtonDown( false )
	{
		for( int i = 0; i < 256; ++i )
		{
			m_keyboard[i] = false;
		}
	}
	//----------------------------------------------------------------------
	MaterialEditorCamera::~MaterialEditorCamera()
	{}
	//----------------------------------------------------------------------
	bool MaterialEditorCamera::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		if( keyCode >= 0 && keyCode < 256 )
		{
			m_keyboard[keyCode] = true;	
			used = true;
		}
		return used;
	}
	//----------------------------------------------------------------------
	bool MaterialEditorCamera::onKeyUp( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		if( keyCode >= 0 && keyCode < 256 )
		{
			m_keyboard[keyCode] = false;	
			used = true;
		}
		return used;
	}
	//----------------------------------------------------------------------
	bool MaterialEditorCamera::onMouseButton( int mouseKeyCode, MonkyMouseButtonState state, int x, int y )
	{ 
		m_leftButtonDown = ( mouseKeyCode == MONKY_BUTTON_LEFT ) && ( state == MONKY_MOUSE_BUTTON_DOWN );
		m_rightButtonDown = ( mouseKeyCode == MONKY_BUTTON_RIGHT ) && ( state == MONKY_MOUSE_BUTTON_DOWN );
		m_mousePosWhenMouseButtonLastWentDown = vec2i( x, y );
		return false; 
	}
	//----------------------------------------------------------------------
	void MaterialEditorCamera::onMouseMove( int x, int y )
	{
		if( m_rightButtonDown )
		{
			vec3f dirToMove = getWorldPosition() - m_lookAtPos;
			float currentDist = dirToMove.length();
			dirToMove /= currentDist;
			float deltaY = (float)( y - m_mousePosWhenMouseButtonLastWentDown.y );

			float newDist = currentDist + deltaY*0.1f;
			newDist = MathFuncs<float>::clamp( newDist, 20.0f, 100.0f );

			m_frames[ m_currentFrame ].position = m_lookAtPos + dirToMove * newDist;
			m_viewMatrix = mat4f::lookAt( getWorldPosition(), m_lookAtPos ).inverse();
		}
		else if( m_leftButtonDown )
		{
			vec3f dirToMove = getWorldPosition() - m_lookAtPos;
			float currentDist = dirToMove.length();
			dirToMove /= currentDist;
			float deltaX = (float)( x - m_mousePosWhenMouseButtonLastWentDown.x );
			//float deltaY = y - m_mousePosWhenMouseButtonLastWentDown.y;
			mat4f rot( mat4f::IDENTITY );
			//rot.rotateX( deltaY * 0.001f );
			rot.rotateY( deltaX * 0.001f );
			vec3f newDir = transformVector( dirToMove, rot );

			m_frames[ m_currentFrame ].position = m_lookAtPos + newDir * currentDist;
			m_viewMatrix = mat4f::lookAt( getWorldPosition(), m_lookAtPos ).inverse();

		}
		m_mousePosWhenMouseButtonLastWentDown = vec2i( x, y );
	}
	//----------------------------------------------------------------------
	void MaterialEditorCamera::updateInput()
	{

	}
	//----------------------------------------------------------------------
	void MaterialEditorCamera::update( double deltaTime )
	{
		//m_viewMatrix = mat4f::lookAt( getWorldPosition(), m_lookAtPos ).inverse();
	}
}