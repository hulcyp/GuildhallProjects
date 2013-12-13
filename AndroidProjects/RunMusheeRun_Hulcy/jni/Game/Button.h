#pragma once
#include "CoreEngine/CommonCoreEngineIncludes.h"


namespace Monky
{
	class Mesh;

	class Button
	{
	public:
		Button( const aabb2f& boundingBox, const std::string& upMaterial );
		void SetDownMaterial( const std::string& downMaterial );

		void SetEventToFire( const std::string& eventName );
		void SetEventToFireWithParams( const std::string& eventName, NamedProperties& params );
		void FingerDown( const vec2f& fingerPos );
		void FingerUp( const vec2f& fingerPos );

		void Render();

		void ReloadMesh();

	private:
		aabb2f m_boundingBox;
		Mesh* m_mesh;
		std::string m_upMaterial;
		std::string m_downMaterial;
		std::string m_eventToFire;
		NamedProperties m_params;
	};
}

