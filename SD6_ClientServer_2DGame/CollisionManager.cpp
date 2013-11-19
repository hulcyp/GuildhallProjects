#include "CollisionManager.h"
#include "Spaceship.h"
#include <algorithm>

namespace Monky
{
	CollisionManager::CollisionManager()
	{
		registerForEvent( "addSGAToCollisionMgr", *this, &CollisionManager::addSGAEvt );
		registerForEvent( "removeSGAFromCollisionMgr", *this, &CollisionManager::removeSGAEvt );
	}
	//----------------------------------------------------------------------
	CollisionManager::~CollisionManager()
	{}
	//----------------------------------------------------------------------
	void CollisionManager::addSGA( SpaceGameActor* sga )
	{
		m_spaceGameActors.push_back( sga );
	}
	//----------------------------------------------------------------------
	void CollisionManager::removeSGA( SpaceGameActor* sga )
	{
		sga;
		m_spaceGameActors.erase( 
			std::remove_if( m_spaceGameActors.begin(), m_spaceGameActors.end(),
			[&] ( SpaceGameActor* ship )
			{
				return sga == ship;
			} ), m_spaceGameActors.end() );
	}
	//----------------------------------------------------------------------
	void CollisionManager::addSGAEvt( NamedProperties& params )
	{
		SpaceGameActor* sga;
		if( params.get( "SGA", sga ) == NamedProperties::SUCCESS )
			addSGA( sga );
	}
	//----------------------------------------------------------------------
	void CollisionManager::removeSGAEvt( NamedProperties& params )
	{
		SpaceGameActor* sga;
		if( params.get( "SGA", sga ) == NamedProperties::SUCCESS )
			removeSGA( sga );
	}
	//----------------------------------------------------------------------
	void CollisionManager::udpate( double deltaTime )
	{
		if( !m_spaceGameActors.empty() )
		{
			for( auto outer = m_spaceGameActors.begin(); ; ++outer )
			{
				auto next = outer;
				++next;
				if( next == m_spaceGameActors.end() )
					break;

				for( auto inner = next; inner != m_spaceGameActors.end(); ++inner )
				{
					if( !(*outer)->canCollideWith( *inner ) )
					{
						continue;	
					}

					const float totalRadius = (*outer)->getRadius() + (*inner)->getRadius();
					const float totalRadiusSquared = totalRadius * totalRadius;

					const vec3f oPos = (*outer)->getPosition();
					const vec3f iPos = (*inner)->getPosition();

					const vec3f delta = oPos - iPos;

					const float distSqrd = delta.lengthSqrd();

					if( distSqrd < totalRadiusSquared )
					{
						(*outer)->onCollision( *inner );
						(*inner)->onCollision( *outer );
					}
				}
			}
		}
	}
}