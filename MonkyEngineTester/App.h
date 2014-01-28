#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/14/2014
//	Description:
//
//---------------------------------------------
#include "GameApp.h"

namespace Monky
{
	class Mesh;
	class MaterialGenerator;

	class App : public GameApp
	{
	public:
		App( double deltaTime, int width, int height, float fov, const char* windowTitle );
		virtual ~App();

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );

		
	protected:
		virtual void updateDisplay();
	private:
		Mesh* m_cube;
		Mesh* m_icosahedron;
		int m_subdivisions;
		MaterialGenerator* m_materialGenerator;
	};
}