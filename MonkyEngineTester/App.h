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
	struct Light;

	class App : public GameApp
	{
	public:
		App( double deltaTime, int width, int height, float fov, const char* windowTitle );
		virtual ~App();

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );
		virtual void onFileDrop( const std::string& filePath );

		
	protected:
		virtual void updateDisplay();
	private:
		Mesh* m_icosahedron;
		int m_subdivisions;
		MaterialGenerator* m_materialGenerator;
		std::string m_currentMatBeingTestedName;
		std::string m_currentMatBeingTestedPath;
		bool m_unlit;
		Light* m_light;
		Light* m_dudLight;
	};
}