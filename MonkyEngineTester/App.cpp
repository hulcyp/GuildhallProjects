#include "App.h"
#include "Renderer.h"
#include "MeshFactory.h"
#include "MaterialGenerator.h"
#include <glew.h>
#include <gl/GL.h>
#include "Material.h"

namespace Monky
{

	App::App( double deltaTime, int width, int height, float fov, const char* windowTitle )
		:	GameApp( deltaTime, width, height, fov, windowTitle )
		,	m_subdivisions( 4 )
		,	m_currentMatBeingTestedName( "GreenGlowMaterial" )
		,	m_currentMatBeingTestedPath( "./materials/GreenGlowMat.xml" )
		,	m_unlit( false )
	{
		showCursor( true );
		m_materialGenerator = new MaterialGenerator();

		m_materialGenerator->GenerateMaterial( m_currentMatBeingTestedPath );

		m_icosahedron = MeshFactory::generateIcosahedron( 5.0f, m_subdivisions, m_currentMatBeingTestedName, Color4f(1.f,1.f,1.f,0.f) );

		m_light = new Light();
		m_light->direction = vec3f( 0.0f, -1.0f, 0.0f );
		m_light->color = color::WHITE;
		m_icosahedron->addLight( m_light, 0 );

		m_dudLight = new Light();
		for( int i = 1; i < MAX_NUM_LIGHTS; ++i )
		{
			m_icosahedron->addLight( m_dudLight, i );
		}
	}
	//----------------------------------------------------------------------
	App::~App()
	{
		SAFE_DELETE( m_icosahedron );
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
				m_icosahedron = MeshFactory::generateIcosahedron( 5.0f, MathFuncs<int>::clamp( ++m_subdivisions, 0, 7 ), m_currentMatBeingTestedName, Color4f(1.f,1.f,1.f,0.f) );
					used = true;
				break;
			case MONKY_PAGEDOWN:
				SAFE_DELETE( m_icosahedron );
				m_icosahedron = MeshFactory::generateIcosahedron( 5.0f, MathFuncs<int>::clamp( --m_subdivisions, 0, 7 ), m_currentMatBeingTestedName, Color4f(1.f,1.f,1.f,0.f) );
				used = true;
				break;
			case MONKY_m:
				m_materialGenerator->GenerateMaterial( m_currentMatBeingTestedPath );
				used = true;
				break;
			case MONKY_u:
				m_unlit = !m_unlit;
				m_renderer->enableLighting( !m_unlit );
			}
		}
		return used;
	}
	//----------------------------------------------------------------------
	void App::onFileDrop( const std::string& filePath )
	{
		std::vector< std::string > tokens;
		stringTokenizer( filePath, tokens, "\\" );
		std::string parseFilePath;
		if( tokens.size() >= 2 );
		{
			parseFilePath = ".\\" + tokens[ tokens.size() - 2 ] + "\\" + tokens[ tokens.size() - 1 ]; 
		}

		if( parseFilePath != "" )
		{
			Material* mat = m_materialGenerator->GenerateMaterial( parseFilePath );
			if( mat != nullptr )
			{
				m_currentMatBeingTestedName = mat->getMaterialName();
				m_currentMatBeingTestedPath = parseFilePath;
				m_icosahedron->setMaterial( m_currentMatBeingTestedName );
				m_icosahedron->addLight( m_light, 0 );

				m_dudLight = new Light();
				for( int i = 1; i < MAX_NUM_LIGHTS; ++i )
				{
					m_icosahedron->addLight( m_dudLight, i );
				}
			}
		}
	}
	//----------------------------------------------------------------------
	void App::updateDisplay()
	{

		GameApp::updateDisplay();

		m_renderer->renderAxis( 10.0f );
		
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		mat4f rotate( mat4f::IDENTITY );
		rotate.rotateY( MathFuncs<float>::PI * 0.5f );
		m_renderer->getModelStack().pushMatrix( rotate );
		m_renderer->renderMesh( m_icosahedron );
		m_renderer->getModelStack().pop();

		GameApp::renderHUD();
		GameApp::renderDebugHUD();
		GameApp::renderConsole();
	}


}
