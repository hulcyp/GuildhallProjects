#include "App.h"
#include "Renderer.h"
#include "MeshFactory.h"
#include "MaterialGenerator.h"

namespace Monky
{

	App::App( double deltaTime, int width, int height, float fov, const char* windowTitle )
		:	GameApp( deltaTime, width, height, fov, windowTitle )
		,	m_subdivisions( 1 )
		,	m_currentMatBeingTestedName( "GreenGlowMaterial" )
		,	m_currentMatBeingTestedPath( "./materials/GreenGlowMat.xml" )
	{
		m_materialGenerator = new MaterialGenerator();

		m_materialGenerator->GenerateMaterial( m_currentMatBeingTestedPath );

		m_cube = MeshFactory::generateCube( 10.0f );
		m_icosahedron = MeshFactory::generateIcosahedron( 5.0f, m_subdivisions, m_currentMatBeingTestedName );

	}
	//----------------------------------------------------------------------
	App::~App()
	{

	}
	//----------------------------------------------------------------------
	bool App::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		used = GameApp::onKeyDown( keyCode, x, y );
		if( !used )
		{
			switch( keyCode )
			{
			case MONKY_PAGEUP:
				SAFE_DELETE( m_icosahedron );
				m_icosahedron = MeshFactory::generateIcosahedron( 5.0f, MathFuncs<int>::clamp( ++m_subdivisions, 0, 7 ), m_currentMatBeingTestedName );
				used = true;
				break;
			case MONKY_PAGEDOWN:
				SAFE_DELETE( m_icosahedron );
				m_icosahedron = MeshFactory::generateIcosahedron( 5.0f, MathFuncs<int>::clamp( --m_subdivisions, 0, 7 ), m_currentMatBeingTestedName );
				used = true;
				break;
			case MONKY_m:
				m_materialGenerator->GenerateMaterial( m_currentMatBeingTestedPath );
				used = true;
				break;
			}
		}
		return used;
	}
	//----------------------------------------------------------------------
	void App::updateDisplay()
	{
		GameApp::updateDisplay();
		m_renderer->renderAxis( 10.0f );
		m_renderer->renderMesh( m_icosahedron );
		
		GameApp::renderHUD();
		GameApp::renderDebugHUD();
		GameApp::renderConsole();
	}


}
