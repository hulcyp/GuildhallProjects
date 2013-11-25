#include "ConsoleInterface.h"
#include <android/log.h>

#define LOG_TAG "MonkyEngine"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

namespace Monky
{
	ConsoleSystem * initConsole( int width, int height, int bufferSize )
	{ 
		return nullptr; 
	}
	//----------------------------------------------------------------------
	void cleanupConsole()
	{}
	//----------------------------------------------------------------------
	void consolePrintColorf( const char* format, Color4f color, ... )
	{
		va_list args;
		va_start( args, color );
		vConsolePrintf( format, color, args );
	}
	//----------------------------------------------------------------------
	void vConsolePrintf( const char* format, Color4f color, va_list args )
	{
		char text[256];
		vsprintf( text, format, args );
		va_end( args );
		std::string str = text;
		str += '\n';
		LOGI( str.c_str() );
	}
	//----------------------------------------------------------------------
	void consolePrintf( const char* format, ... )
	{
		va_list args;
		va_start( args, format );
		vConsolePrintf( format, color::WHITE, args );
	}
	//----------------------------------------------------------------------
	void executeConsoleCommand( const std::string& cmd, const std::string& args )
	{}
	//----------------------------------------------------------------------
	void registerConsoleCommand( const std::string& cmd, const std::string& helpText, std::function< void( const std::string& ) > cmdFunc )
	{}
}
