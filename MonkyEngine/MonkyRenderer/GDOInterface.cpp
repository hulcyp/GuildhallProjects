#include "GDOInterface.h"
#include "GDOManager.h"
#include "ConsoleInterface.h"
#include "GDOPoint.h"
#include "GDOSphere.h"
#include "GDOOrientation.h"
#include "GDOArrow.h"

namespace Monky
{
	GDOManager* g_GDOManager = nullptr; 

	void clearAllGDOsCmd( const std::string& args );
	
	GDOManager* initGDOManager()
	{
		if( !g_GDOManager )
		{
			g_GDOManager = new GDOManager();
			registerConsoleCommand( "clearGDOs", "Clears all active GDOs", clearAllGDOsCmd );
			return g_GDOManager;
		}
		return nullptr;
	}

	void cleanupGDOManager()
	{
		SAFE_DELETE( g_GDOManager );
	}

	void createPointGDO( float radius, const vec3f& pos, float showUntilTime, Color4f color )
	{
		if( g_GDOManager != nullptr )
			g_GDOManager->addGDO( new GDOPoint( radius, pos, showUntilTime, color ) );
	}

	void createSphereGDO( float radius, int rings, int sectors, const vec3f& pos, float showUntilTime, Color4f color )
	{
		if( g_GDOManager != nullptr )
			g_GDOManager->addGDO( new GDOSphere( radius, rings, sectors, pos, showUntilTime, color ) );
	}

	void createOrientationGDO( float size, const vec3f& pos, const mat3f& rotation, Color4f color, float showUntilTime )
	{
		if( g_GDOManager != nullptr )
			g_GDOManager->addGDO( new GDOOrientation( size, pos, rotation, color, showUntilTime ) );
	}

	void createOrientationGDO( float size, const vec3f& pos, const mat3f& rotation, float showUntilTime )
	{
		if( g_GDOManager != nullptr )
			g_GDOManager->addGDO( new GDOOrientation( size, pos, rotation, showUntilTime ) );
	}

	void createArrowGDO( const vec3f& dir, float length, const vec3f& pos, float showUntilTime )
	{
		if( g_GDOManager != nullptr )
			g_GDOManager->addGDO( new GDOArrow( dir, length, pos, showUntilTime ) );
	}
	
	void clearAllGDOsCmd( const std::string& args )
	{
		if( g_GDOManager != nullptr )
			g_GDOManager->clearAllGDOs();
	}
}