#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/21/2013
//	Description: Simple interface for making an object collidable using circle collision
//
//---------------------------------------------
#include <CommonEngineIncludes.h>

namespace Monky
{
#define PLAYER_COLLISION_MASK 1
#define PROJECTILE_COLLISION_MASK 2


	class CollisionListener
	{
	public:
		CollisionListener();
		~CollisionListener();

		bool collidesWith( CollisionListener& listenerToCheck ) const;

		virtual void onCollisionEnter( CollisionListener& collider );

	protected:
		vec2f m_position;
		float m_radius;
		bool m_isCollidable;
		short m_collisionGroupBits;		
	};
}