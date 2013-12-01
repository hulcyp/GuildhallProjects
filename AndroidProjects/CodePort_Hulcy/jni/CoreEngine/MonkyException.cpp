#include "MonkyException.h"
//#include <windows.h>
#include <stdio.h>
#include "CoreEngine/ConsoleInterface.h"

#include <android/log.h>

#define LOG_TAG "MonkyEngine"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

namespace Monky
{
	MonkyException::MonkyException( const char* message, const char* filePath, int lineNum )
		:	m_message( message )
		,	m_filePath( filePath )
		,	m_lineNum( lineNum )
	{}
	//------------------------------------------------------------
	const char* MonkyException::what() const throw()
	{
		return m_message;
	}
	//------------------------------------------------------------
	const char* MonkyException::getFilePath() const
	{
		return m_filePath;
	}
	//------------------------------------------------------------
	int MonkyException::getLineNum() const
	{
		return m_lineNum;
	}
	//------------------------------------------------------------
	void MonkyException::printWhat() const
	{
		fatalErrorMessageBox( "Monky Exception", "%s \nException occured at: %s(%d)", m_message, m_filePath, m_lineNum );
	}
	
	//------------------------------------------------------------
	// Static member functions
	//------------------------------------------------------------
	void MonkyException::simpleErrorMessageBox( const char* title, const char* format, ... )
	{
		char msg[1024];
		va_list args;
		va_start( args, format );
		vsprintf( msg, format, args );
		va_end( args );
		//MessageBoxA( NULL, msg, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
	}
	//------------------------------------------------------------
	void MonkyException::fatalErrorMessageBox( const char* title, const char* format, ... )
	{
		char msg[1024];
		va_list args;
		va_start( args, format );
		vsprintf( msg, format, args );
		va_end( args );
		//MessageBoxA( NULL, msg, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
		//exit( -1 );
	}
	//------------------------------------------------------------
	void MonkyException::printToCompilerOutputConsole( const char* error )
	{
		//OutputDebugStringA( error );
		LOGE( error );
	}
	//------------------------------------------------------------
	bool MonkyException::yesNoMessageBox( const char* title, const char* msg )
	{
		bool yesSelected = false;
		/*int selected = MessageBoxA( NULL, msg, title, MB_YESNO | MB_ICONWARNING | MB_SETFOREGROUND );
		switch( selected )
		{
		case IDYES:
			yesSelected = true;
			break;
		case IDNO:
			yesSelected = false;
			break;
		}*/

		return yesSelected;
	}
	void MonkyException::debuggerBreakPoint()
	{
#ifdef MONKY_DEBUG
		__debugbreak();	
#endif
	}
}
