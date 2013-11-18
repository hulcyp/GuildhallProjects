#include "Camera.h"
#include "CoreEngine/MonkyException.h"
#include "MathLib/MathFuncs.h"

namespace Monky
{
	Camera::Camera( const std::string& id, const vec3f& pos, ProjectionMode projMode, const mat4f& projMatrix, const mat4f& viewMatrix  )
		:	Node( id )
		,	m_projectionMatrix( projMatrix )
		,	m_viewMatrix( viewMatrix )
		,	m_projectionMode( projMode )
	{
		setPosition( pos );
	}
	//-------------------------------------------
	Camera& Camera::operator=( const Camera& rhs )
	{
		if( this != &rhs )
		{
			m_id = rhs.m_id;
			m_projectionMatrix = rhs.m_projectionMatrix;
			m_viewMatrix = rhs.m_viewMatrix;
			m_projectionMode = rhs.m_projectionMode;
			setPosition( rhs.getLocalPosition() );
			m_dir = rhs.m_dir;
			m_up = rhs.m_up;
		}
		return *this;
	}
	//-------------------------------------------
	Camera::~Camera()
	{}
	//-------------------------------------------
	void Camera::update( double deltaTime )
	{	
		Node::update( deltaTime );
		m_dir = transformVector( vec3f( 0.0f, 0.0f, -1.0f ), getLocalRotation() );
	}
	//-------------------------------------------
	const mat4f& Camera::getViewMatrix()
	{
		return m_viewMatrix;
	}
	//-------------------------------------------
	const mat4f& Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

}
