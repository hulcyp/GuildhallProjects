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
		,	m_teamNumber( teamNumber )
		,	m_wasSpawnedLastFrame( true )
		,	m_stateTimeline( id )
	{
		NamedProperties params;
		params.set( "SGA", this );
		fireEvent( "addSGAToCollisionMgr", params );
		m_stateTimeline.Set( 0, SGA_State() );
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
		SGA_State currentState = m_stateTimeline.Get( INPUT_DELAY_TIME );
		currentState.rot += ROTATION_POWER;		
		m_stateTimeline.Set( INPUT_DELAY_TIME, currentState );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::applyCounterClockwiseRotation()
	{
		SGA_State currentState = m_stateTimeline.Get( INPUT_DELAY_TIME );
		currentState.rot -= ROTATION_POWER;
		m_stateTimeline.Set( INPUT_DELAY_TIME, currentState );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::applyForwardImpulse()
	{
		vec3f impulseInDirOfShip = transformVector( vec3f( 0.0f, IMPULSE_POWER, 0.0f ), getLocalRotation() );
		SGA_State currentState = m_stateTimeline.Get( INPUT_DELAY_TIME );
		currentState.vel += vec2f( impulseInDirOfShip.x, impulseInDirOfShip.y );
		m_stateTimeline.Set( INPUT_DELAY_TIME, currentState );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::applyReverseImpulse()
	{
		m_forwardImpulse -= 1.0f;
		vec3f impulseInDirOfShip = transformVector( vec3f( 0.0f, -IMPULSE_POWER, 0.0f ), getLocalRotation() );
		SGA_State currentState = m_stateTimeline.Get( INPUT_DELAY_TIME );
		currentState.vel += vec2f( impulseInDirOfShip.x, impulseInDirOfShip.y );
		m_stateTimeline.Set( INPUT_DELAY_TIME, currentState );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::update( double deltaTime )
	{
		Actor::update( deltaTime );
		m_wasSpawnedLastFrame = false;
		vec3f tempPos;
		vec2f tempVel;
		SGA_State currentState = m_stateTimeline.Get( 0 );


		float dt = static_cast< float >( deltaTime );

		setRotation( mat3f::createMatrixFromQuaternion( quatf::makeQuaternionFromAxisAngle( MathFuncs<float>::degreesToRadians( currentState.rot ), vec3f( 0.0f, 0.0f, 1.0f ) ) ) );
		
		currentState.pos += currentState.vel * dt;
		
		currentState.vel -= currentState.vel * m_drag * dt;
		
		currentState.pos.x = (float)MathFuncs<int>::wrap( (int)currentState.pos.x, 0, SCREEN_WIDTH );
		currentState.pos.y = (float)MathFuncs<int>::wrap( (int)currentState.pos.y, 0, SCREEN_HEIGHT );

		m_frames[ m_currentFrame ].position = currentState.pos;
		

		vec3f tempDir = transformVector( vec3f( 0.0f, 1.0f, 0.0f ), getLocalRotation() );
		tempDir.normalize();
		currentState.dir = vec2f( tempDir.x, tempDir.y );

		//m_stateTimeline.Set( 0, currentState );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::UpdatePosFromTimeline()
	{
		m_wasSpawnedLastFrame = false;

		SGA_State currentState = m_stateTimeline.Get( 0 );

		m_frames[ m_currentFrame ].position = currentState.pos;
		m_frames[ m_currentFrame ].position.x = (float)MathFuncs<int>::wrap( (int)m_frames[ m_currentFrame ].position.x, 0, SCREEN_WIDTH );
		m_frames[ m_currentFrame ].position.y = (float)MathFuncs<int>::wrap( (int)m_frames[ m_currentFrame ].position.y, 0, SCREEN_HEIGHT );
	}
	//----------------------------------------------------------------------
	Monky::vec2f SpaceGameActor::getVelocity() const
	{
		return m_stateTimeline.Get( 0 ).vel;
	}
	//----------------------------------------------------------------------
	float SpaceGameActor::getRadius() const
	{
		return m_radius;
	}
	//----------------------------------------------------------------------
	float SpaceGameActor::getSGARotation() const 
	{ 
		return m_stateTimeline.Get( 0 ).rot; 
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::setSGARotation( float rotation ) 
	{ 
		SGA_State currentState = m_stateTimeline.Get( 0 );
		currentState.rot = rotation; 
		m_stateTimeline.Set( 0, currentState );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::setPosition( const vec3f& pos, int frame )
	{
		Object::setPosition( pos, frame );
		SGA_State currentState = m_stateTimeline.Get( 0 );
		currentState.pos = vec2f( pos.x, pos.y );
		m_stateTimeline.Set( 0, currentState );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::setVeloctiy( const vec2f& vel )
	{
		SGA_State currentState = m_stateTimeline.Get( 0 );
		currentState.vel = vel;
		m_stateTimeline.Set( 0, currentState );
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
	void SpaceGameActor::SetSocketForTimeline( SocketUDP* socket )
	{
		m_stateTimeline.SetSocket( socket );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::SendTimelineUpdate( int packetID )
	{
		m_stateTimeline.SendUpdate( packetID );
	}
	//----------------------------------------------------------------------
	void SpaceGameActor::AddTimelineUpdateToPacket( Packet* packet )
	{
		m_stateTimeline.AddUpdateToPacket( packet );
	}
}