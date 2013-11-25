#include "MSGame.h"
#include "MonkyRenderer/SpriteAnimation.h"
#include "MonkyRenderer/SpriteActor.h"

namespace Monky
{
	MSGame::MSGame( double deltaTime, int width, int height, float fov )
		:	GameApp( deltaTime, width, height, fov )
	{}

	MSGame::~MSGame()
	{

	}

	void MSGame::initialize()
	{
		GameApp::initialize();
		m_spinningCube = new SpriteActor("Mushee", 100.0f, "MusheesRunMat" );
		SpriteAnimation* animation = new SpriteAnimation( "animations/MusheesYellow.xml" );
		m_spinningCube->AddAnimation( "Run", animation );
		m_spinningCube->PlayAnimation( "Run" );
		spawn( m_spinningCube );
		m_spinningCube->setPosition( vec3f( m_screenWidth / 2, m_screenHeight / 2, 0.0f ) );
	}

	void MSGame::updateSimulation()
	{
		GameApp::updateSimulation();
	}



}
