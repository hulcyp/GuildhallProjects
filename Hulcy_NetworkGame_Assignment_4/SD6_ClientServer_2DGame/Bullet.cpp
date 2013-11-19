#include "Bullet.h"
#include "MeshFactory.h"
#include "Spaceship.h"

namespace Monky
{
	Bullet::Bullet( const std::string& id, const vec2f& pos, float rotation, short teamNumber, int screenWidth, int screenHeight, bool isServerSide )
		:	SpaceGameActor( id + "_" + toString( sm_nextBulletID++ ), teamNumber, screenWidth, screenHeight )
		,	BULLET_SPEED( 500.0f )
	{
		m_actorType = BULLET_ACTOR_TYPE;
		setPosition( pos );
		m_lifespan = 2.0f;
		m_drag = 0.0f;
		m_radius = 4.0f;
		SGA_State currentState;
		currentState.pos = pos;
		currentState.rot = rotation;
		m_stateTimeline.SetDrag( m_drag );

		const float radians = MathFuncs<float>::degreesToRadians( rotation );
		const float sinRotation = std::sin( radians );
		const float cosRotation = std::cos( radians );

		vec2f dir = vec2f( sinRotation, cosRotation );
		currentState.vel = dir * BULLET_SPEED;		

		if( !isServerSide )
			setMesh( MeshFactory::generateLine( vec2f(), vec2f( 0.0f, 4.0f ), "SimpleColorMaterial", getTeamColor( teamNumber ) ) );
		
		setRotation( mat3f::createMatrixFromQuaternion( quatf::makeQuaternionFromAxisAngle( MathFuncs<float>::degreesToRadians( rotation ), vec3f( 0.0f, 0.0f, 1.0f ) ) ) );

		m_stateTimeline.Set( 0, currentState );
		m_actorTypesCanCollideWith = SPACE_SHIP_ACTOR_TYPE;
	}
	//----------------------------------------------------------------------
	Bullet::~Bullet()
	{

	}
	//----------------------------------------------------------------------
	void Bullet::update( double deltaTime )
	{
		Actor::update( deltaTime );
		m_wasSpawnedLastFrame = false;
		vec3f tempPos;
		vec2f tempVel;
		SGA_State currentState = m_stateTimeline.Get( 0 );
		float dt = static_cast< float >( deltaTime );

		currentState.pos += currentState.vel * dt;


		currentState.pos.x = (float)MathFuncs<int>::wrap( (int)currentState.pos.x, 0, SCREEN_WIDTH );
		currentState.pos.y = (float)MathFuncs<int>::wrap( (int)currentState.pos.y, 0, SCREEN_HEIGHT );

		m_frames[ m_currentFrame ].position = currentState.pos;

		m_stateTimeline.Set( 0, currentState );
	}
	//----------------------------------------------------------------------
	int Bullet::sm_nextBulletID = 0;
}