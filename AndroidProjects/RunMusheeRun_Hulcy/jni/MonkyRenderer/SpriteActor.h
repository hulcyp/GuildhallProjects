#pragma once
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include <string>
#include <map>
#include "Actor.h"

namespace Monky
{
	class SpriteAnimation;
	class Texture;

	class SpriteActor : public Actor
	{
	public:
		SpriteActor( const std::string& name, float radius, const std::string& material, float drag = 0.0f, float mass = 1.0f );
		virtual ~SpriteActor();

		virtual void ApplyImpulse( const vec2f& impulse );
		virtual void ApplyGravity( const vec2f& gravity );
		virtual void update( double deltaTime );
		virtual bool OnCollision( SpriteActor* other );


		vec2f GetVel() { return m_vel; }
		float GetMass() const { return m_mass; }
		float GetRadius() const { return m_radius; }


		virtual bool IsVisible() { return m_visible; }
		virtual void SetVisible( bool visible );
		virtual void render();


		//Animation
		void AddAnimation( const std::string& animationName, SpriteAnimation* animation );
		virtual void PlayAnimation( const std::string& animationName );
		const std::string& GetCurrentAnimationName();
		bool HasAnimation( const std::string& animationName );

	protected:
		void Init();
		void GenerateMesh( float width, float height, const std::string& materialName);


	protected:
			vec2f m_vel;
			float m_drag;
			float m_mass;
			float m_radius;
			bool m_visible;
			std::map< std::string, SpriteAnimation* > m_animations;
			std::string m_currentAnimationName;
			SpriteAnimation* m_currentAnimation;

	};
}
