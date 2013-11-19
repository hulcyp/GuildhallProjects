#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/21/2013
//	Description:
//
//---------------------------------------------
#include "Actor.h"
#include "SGA_StateTimeline.h"

#define SPACE_SHIP_ACTOR_TYPE 1
#define BULLET_ACTOR_TYPE 4

namespace Monky
{
	class SpaceGameActor : public Actor
	{
	public:
		SpaceGameActor( const std::string& id, short teamNumber, int screenWidth, int screenHeight );
		virtual ~SpaceGameActor();
	
		virtual void render();
		virtual void renderDebug();

		void applyClockwiseRotation();
		void applyCounterClockwiseRotation();

		void applyForwardImpulse();
		void applyReverseImpulse();

		virtual void update( double deltaTime );
		void UpdatePosFromTimeline();

		vec2f getVelocity() const;
		float getRadius() const;
		float getSGARotation() const;
		void setSGARotation( float rotation );

		virtual void setPosition( const vec3f& pos, int frame = 0 );
		void setVeloctiy( const vec2f& vel );

		void onCollision( SpaceGameActor* other );

		short getTeamNumber() const { return m_teamNumber; }
		bool didSpawnLastFrame() const { return m_wasSpawnedLastFrame; }

		bool canCollideWith( SpaceGameActor* other );

		void SetSocketForTimeline( SocketUDP* socket );
		void SendTimelineUpdate( int packetID );
		void AddTimelineUpdateToPacket( Packet* packet );

		const std::string& GetTimelineName() const { return m_stateTimeline.GetName(); }

	protected:	
		SGA_StateTimeline m_stateTimeline;
		
		short m_teamNumber;
		int m_actorTypesCanCollideWith;
		bool m_wasSpawnedLastFrame;
		bool m_isServerSide;
		float m_forwardImpulse;
		float m_drag;
		float m_radius;
		const float IMPULSE_POWER;
		const float ROTATION_POWER;	
		const int SCREEN_WIDTH;
		const int SCREEN_HEIGHT;
	};
}