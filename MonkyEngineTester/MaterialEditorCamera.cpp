#include "MaterialEditorCamera.h"

namespace Monky
{

	MaterialEditorCamera::MaterialEditorCamera( const std::string& id, const vec3f& pos, const vec3f& lookAt, const mat4f& projMatrix, const mat4f& viewMatrix /*= mat4f::IDENTITY */ )
		:	Camera( id, pos, PH_PERSPECTIVE, projMatrix, viewMatrix )
		,	InputListener( true )
		,	m_lookAtPos( lookAt )
	{

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
	void MaterialEditorCamera::onMouseMove( int x, int y )
	{

	}
	//----------------------------------------------------------------------
	void MaterialEditorCamera::updateInput()
	{

	}
	//----------------------------------------------------------------------
	void MaterialEditorCamera::update( double deltaTime )
	{
		m_viewMatrix = mat4f::lookAt( getWorldPosition(), m_lookAtPos );
	}
}