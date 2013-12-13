#pragma once
#include "MonkyRenderer/Camera.h"

namespace Monky
{
	class MSCamera : public Camera
	{
	public:
		MSCamera( const std::string& id, const vec3f& pos, ProjectionMode projMode, const mat4f& projMatrix, const mat4f& viewMatrix = mat4f::IDENTITY );

		virtual void update( double deltaTime );

	};
}
