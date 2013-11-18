#pragma once

#include"CommonCoreEngineIncludes.h"
#include "Node.h"

namespace Monky
{
	class Camera : public Node
	{
	public:

		enum ProjectionMode
		{
			PH_PERSPECTIVE,
			PH_ORTHOGRAPHIC
		};

		Camera( const std::string& id, const vec3f& pos, ProjectionMode projMode, const mat4f& projMatrix, const mat4f& viewMatrix = mat4f::IDENTITY );
		Camera& operator=( const Camera& rhs );
		virtual ~Camera();		


		virtual void update( double deltaTime );

		vec3f getPosition() const { return getWorldPosition(); }
		vec3f getDirection() const { return m_dir; }
		vec3f getUpVector() const { return m_up; }
		virtual const mat4f& getViewMatrix();
		virtual const mat4f& getProjectionMatrix() const;

		ProjectionMode getProjectionMode() const { return m_projectionMode; }

	protected:
		vec3f m_dir;
		vec3f m_up;
		mat4f m_viewMatrix;
		mat4f m_projectionMatrix;
		ProjectionMode m_projectionMode;
		
		
	};
}