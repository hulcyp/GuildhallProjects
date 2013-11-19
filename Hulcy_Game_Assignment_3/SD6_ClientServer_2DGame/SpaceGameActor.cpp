#include "SpaceGameActor.h"
#include "ClientServerInterface.h"

namespace Monky
{
	SpaceGameActor::SpaceGameActor( const std::string& id, short teamNumber, int screenWidth, int screenHeight )
		:	Actor( id )
		,	SCREEN_WIDTH( screenWidth )
		,	SCREEN_HEIGHT( screenHeight )
		,	m_radius( 0.0f )
		,	IMPULSE_POWER( 100.0f )
		,	ROTATION_POWER( 5.0f )
		,	m_forwardImpulse( 0.0f )
		,	m_rotation( 0.0f )
		,	m_teamNumber( teamNumber )
		,	m_wasSpawnedLastFrame( true )
	{
		NamedProperties params;
		params.set( "SGA", this );
		fireEvent( "addSGAToCollisionMgr", params );
	}
	//----------------------------------------------------------------------
	SpaceGameActor::~SpaceGameActor()
	{
 		NamedProperties params;
		params.set( "SGA", this );
		fireEvent( "removeSGAFromCollisionMgr", params );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::render()
	{
		Object::render();
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::renderDebug()
	{
		Object::renderDebug();
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::applyClockwiseRotation()
	{
		m_rotation += ROTATION_POWER;
		m_forcesAppliedBitFlags |= APPLY_CLOCKWISE_ROTATION;
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::applyCounterClockwiseRotation()
	{
		m_rotation -= ROTATION_POWER;
		m_forcesAppliedBitFlags |= APPLY_COUNTER_CLOCKWISE_ROTATION;
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::applyForwardImpulse()
	{
		vec3f impulseInDirOfShip = transformVector( vec3f( 0.0f, IMPULSE_POWER, 0.0f ), getLocalRotation() );
		
		m_vel += vec2f( impulseInDirOfShip.x, impulseInDirOfShip.y );
		m_forcesAppliedBitFlags |= APPLY_FORWARD_IMPULSE;
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::applyReverseImpulse()
	{
		m_forwardImpulse -= 1.0f;
		vec3f impulseInDirOfShip = transformVector( vec3f( 0.0f, -IMPULSE_POWER, 0.0f ), getLocalRotation() );

		m_vel += vec2f( impulseInDirOfShip.x, impulseInDirOfShip.y );
		m_forcesAppliedBitFlags |= APPLY_REVERSE_IMPULSE;
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::update( double deltaTime )
	{
		Actor::update( deltaTime );
		m_wasSpawnedLastFrame = false;

		float dt = static_cast< float >( deltaTime );

		setRotation( mat3f::createMatrixFromQuaternion( quatf::makeQuaternionFromAxisAngle( MathFuncs<float>::degreesToRadians( m_rotation ), vec3f( 0.0f, 0.0f, 1.0f ) ) ) );

		//vec3f impulseInDirOfShip = transformVector( vec3f( 0.0f, IMPULSE_POWER, 0.0f ), getLocalRotation() );
		//impulseInDirOfShip *= m_forwardImpulse;
		//
		//m_vel += vec2f( impulseInDirOfShip.x, impulseInDirOfShip.y );
		m_frames[ m_currentFrame ].position += m_vel*dt;	
		m_vel -= m_vel * m_drag * dt;
		//m_forwardImpulse = 0.0f;

		m_frames[ m_currentFrame ].position.x = (float)MathFuncs<int>::wrap( (int)m_frames[ m_currentFrame ].position.x, 0, SCREEN_WIDTH );
		m_frames[ m_currentFrame ].position.y = (float)MathFuncs<int>::wrap( (int)m_frames[ m_currentFrame ].position.y, 0, SCREEN_HEIGHT );

		vec3f tempDir = transformVector( vec3f( 0.0f, 1.0f, 0.0f ), getLocalRotation() );
		tempDir.normalize();
		m_dir = vec2f( tempDir.x, tempDir.y );
	}
	//----------------------------------------------------------------------
	Monky::vec2f SpaceGameActor::getVelocity() const
	{
		return m_vel;
	}
	//----------------------------------------------------------------------
	float SpaceGameActor::getRadius() const
	{
		return m_radius;
	}
	//----------------------------------------------------------------------
	float SpaceGameActor::getSGARotation() const 
	{ 
		return m_rotation; 
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::setSGARotation( float rotation ) 
	{ 
		m_rotation = rotation; 
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::setVeloctiy( const vec2f& vel )
	{
		m_vel = vel;
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::onCollision( SpaceGameActor* other )
	{
		setLifespan( 0.1f );
	}
	//----------------------------------------------------------------------
	bool SpaceGameActor::canCollideWith( SpaceGameActor* other )
	{
		bool canCollideWith = false;
		if( m_teamNumber != other->m_teamNumber )
		{
			if( ( m_actorTypesCanCollideWith & other->m_actorType ) != 0 )
			{
				canCollideWith = true;		
			}
		}
		return canCollideWith;
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::clearForcesAppliedBitFlags()
	{
		m_forcesAppliedBitFlags = 0;
	}
	//----------------------------------------------------------------------
	short SpaceGameActor::getForcesAppliedBitFlags() const
	{
		return m_forcesAppliedBitFlags;
	}
}