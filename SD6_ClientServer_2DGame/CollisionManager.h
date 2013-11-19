#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/21/2013
//	Description: Collision manager for the spaceship game to manage
//	collision with spaceships and bullets
//---------------------------------------------
#include <CommonEngineIncludes.h>

namespace Monky
{
	class SpaceGameActor;

	class CollisionManager
	{
	public:
		CollisionManager();
		~CollisionManager();

		void addSGA( SpaceGameActor* sga );
		void removeSGA( SpaceGameActor* sga );

		void addSGAEvt( NamedProperties& params );
		void removeSGAEvt( NamedProperties& params );

		void udpate( double deltaTime );

	private:
		std::vector< SpaceGameActor* > m_spaceGameActors;
	};
}