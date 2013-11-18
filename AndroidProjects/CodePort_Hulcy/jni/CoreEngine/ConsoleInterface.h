#pragma once
#include "StdLibraryIncludes.h"
#include "Colors.h"

namespace Monky
{
#ifndef MONKY_CONSOLE
#define MONKY_CONSOLE
	class ConsoleSystem;

	ConsoleSystem * initConsole( int width, int height, int bufferSize );
	void cleanupConsole();
	void consolePrintColorf( const char* format, Color4f color, ... );
	void vConsolePrintf( const char* format, Color4f color, va_list args );
	void consolePrintf( const char* format, ... );
	void executeConsoleCommand( const std::string& cmd, const std::string& args );
	void registerConsoleCommand( const std::string& cmd, const std::string& helpText, std::function< void( const std::string& ) > cmdFunc );
#endif
}