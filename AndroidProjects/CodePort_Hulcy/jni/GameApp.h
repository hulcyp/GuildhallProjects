#pragma once
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "CoreEngine/Clock.h"
#include "CoreEngine/Singleton.h"
#include "CoreEngine/InputSystem.h"
#include <android_native_app_glue.h>

#include <EGL/egl.h>


namespace Monky
{
	class Camera;
	class Renderer;


	struct AndroidInfo
	{
		struct android_app* app;


		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;
	};

	class GameApp : public Singleton<GameApp>
	{
	public:
		GameApp( double deltaTime, int width, int height, float fov, AndroidInfo* androidInfo );
		virtual ~GameApp();
				
		void runApp();

		virtual void initialize();
		virtual void cleanup();
		
		virtual void updateFrame();
		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y );

		virtual void onMouseMove( int mouseX, int mouseY );
		virtual bool onMouseButton( int keyCode, MonkyMouseButtonState state, int mouseX, int mouseY );

		void executeStartUpCmds( const std::string& filePath );
				
		void initializeCamera( Camera* camera );

		void exitProgram( int code );
	
	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();

		virtual void renderHUD();
		virtual void renderDebugHUD();
		void renderConsole();

		void updateFPS();

	protected:
		const double DELTA_TIME;

		AndroidInfo* m_androidInfo;

		Renderer* m_renderer;
		Camera* m_camera;
		Clock m_frameClock;

		Clock m_applicationClock;
		float m_nextTimeFrameIsOwed;
		bool m_frameOwed;
		
		NamedProperties m_mainFontParams;
		NamedProperties m_memoryVisualizerParams;

		float m_currentFPS;
		float m_previousTime;
		int m_frameCount;

		int m_screenWidth;
		int m_screenHeight;

		bool m_renderAxis;
		bool m_renderDebugHUD;

		Camera* m_debugCamera;

		class QuitProgram : public std::exception
		{};
	};
}
