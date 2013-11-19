#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/21/2013
//	Description:
//
//---------------------------------------------
#include "SpaceGameActor.h"

namespace Monky
{
	class Bullet : public SpaceGameActor
	{
	public:
		Bullet( const vec2f& pos, float rotation, short teamNumber, int screenWidth, int screenHeight, bool isServerSide = false );
		virtual ~Bullet();

		float getRotation() const { return m_rotation; }
		float getRadius() const { return m_radius; }

	private:
		float m_radius;		
		const float BULLET_SPEED;

		static int sm_nextBulletID;
	};
}