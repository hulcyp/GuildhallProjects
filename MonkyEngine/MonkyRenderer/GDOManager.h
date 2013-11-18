#pragma once
#include <list>

namespace Monky
{
	class GraphicDebugObject;
	class Renderer;

	class GDOManager
	{
	public:
		GDOManager();
		~GDOManager();

		
		void addGDO( GraphicDebugObject* gdo );
		void renderGDOs( Renderer* renderer, float currentTime );
		float getCurrentTime() { return m_currentTime; }
		void clearAllGDOs();
		

	private:
		std::list< GraphicDebugObject* > m_GDOs;
		float m_currentTime;
	};
}