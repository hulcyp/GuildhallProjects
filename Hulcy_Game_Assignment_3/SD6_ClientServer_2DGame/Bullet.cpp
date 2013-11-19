#include "Bullet.h"
#include "MeshFactory.h"
#include "Spaceship.h"

namespace Monky
{
	Bullet::Bullet( const vec2f& pos, float rotation, short teamNumber, int screenWidth, int screenHeight, bool isServerSide )
		:	SpaceGameActor( toString( sm_nextBulletID++ ), teamNumber, screenWidth, screenHeight )
		,	BULLET_SPEED( 500.0f )
	{
		m_actorType = BULLET_ACTOR_TYPE;
		setPosition( pos );
		m_lifespan = 2.0f;
		m_drag = 0.0f;
		m_radius = 4.0f;
		m_rotation = rotation;

		const float radians = MathFuncs<float>::degreesToRadians( m_rotation );
		const float sinRotation = std::sin( radians );
		const float cosRotation = std::cos( radians );

		vec2f dir = vec2f( sinRotation, cosRotation );
		m_vel = dir * BULLET_SPEED;		

		if( !isServerSide )
			setMesh( MeshFactory::generateLine( vec2f(), vec2f( 0.0f, 4.0f ), "SimpleColorMaterial", getTeamColor( teamNumber ) ) );

		m_actorTypesCanCollideWith = SPACE_SHIP_ACTOR_TYPE;
	}
	//----------------------------------------------------------------------
	Bullet::~Bullet()
	{

	}
	//----------------------------------------------------------------------
	int Bullet::sm_nextBulletID = 0;
}