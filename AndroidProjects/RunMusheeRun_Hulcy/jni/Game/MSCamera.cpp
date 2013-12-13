#include "Game/MSCamera.h"

namespace Monky
{
	MSCamera::MSCamera( const std::string& id, const vec3f& pos, ProjectionMode projMode, const mat4f& projMatrix, const mat4f& viewMatrix )
		:	Camera( id, pos, projMode, projMatrix, viewMatrix )
	{}

	void MSCamera::update( double deltaTime )
	{
		vec3f pos = getWorldPosition();
		m_viewMatrix[12] = -pos.x;
		m_viewMatrix[13] = -pos.y;
		m_viewMatrix[14] = -pos.z;
		Camera::update( deltaTime );
	}
}
