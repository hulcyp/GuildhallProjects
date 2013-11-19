#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "Shader.h"
#include <CoreEngine/MonkyException.h>
#include "CoreEngine/StringUtils.h"
#include "CoreEngine/FileManager.h"
#include <cstring>

namespace Monky
{
	Shader::Shader( const std::string& fileName, GLenum shaderType )
	{
		loadShaderFromFile( fileName.c_str(), shaderType );
	}
	//--------------------------------------------------------------------------------
	Shader::Shader( const char* shader, GLenum shaderType )
	{
		loadShaderFromBuffer( shader, shaderType );
	}
	//--------------------------------------------------------------------------------
	Shader::~Shader()
	{
		if( m_shaderID )
		{
			glDeleteShader( m_shaderID );
			m_shaderID = 0;
		}
	}
	//--------------------------------------------------------------------------------
	Shader* Shader::createOrGetShader( const std::string& fileName, GLenum shaderType )
	{
		consolePrintf( "creating or getting a shader: %s", fileName.c_str() );
		std::map< std::string, Shader* >::iterator iter = sm_shaders.find( fileName );
		consolePrintf( "Search completed" );
		//Shader*& shader = sm_shaders[ fileName ];
		if( iter == sm_shaders.end() )
		{
			iter = sm_shaders.insert( std::pair< std::string, Shader* >( fileName, new Shader( fileName, shaderType ) ) ).first;
		}
		consolePrintf( "insert or retrievel succesful" );
		return iter->second;
	}
	//--------------------------------------------------------------------------------
	void Shader::createShader( const std::string& fileName, GLenum shaderType )
	{
		createOrGetShader( fileName, shaderType );
	}
	void Shader::createShaderFromBuffer( const std::string& shaderName, const char* shader, GLenum shaderType )
	{
		consolePrintf( "creating or getting a shader: %s", shaderName.c_str() );
		std::map< std::string, Shader* >::iterator iter = sm_shaders.find( shaderName );
		consolePrintf( "Search completed" );
		//Shader*& shader = sm_shaders[ fileName ];
		if( iter == sm_shaders.end() )
		{
			sm_shaders.insert( std::pair< std::string, Shader* >( shaderName, new Shader( shader, shaderType ) ) );
		}
		consolePrintf( "insert or retrievel succesful" );
	}
	//--------------------------------------------------------------------------------
	Shader* Shader::getShader( const std::string& fileName )
	{
		std::map< std::string, Shader* >::iterator iter = sm_shaders.find( fileName );
		if( iter != sm_shaders.end() )
			return iter->second;
		else
			return nullptr;
	}
	//--------------------------------------------------------------------------------
	void Shader::cleanupShaders()
	{
		MAP_CLEANUP( sm_shaders );
	}
	//--------------------------------------------------------------------------------
	void Shader::loadShaderFromFile( const char* fileName, GLenum shaderType )
	{
		GLuint shaderID = 0;
		std::string shaderString;
		//std::ifstream shaderFile( fileName );
		int shaderBufferSize = -1;
		const char* shaderBuffer = nullptr;
		consolePrintf( "Loading shader from file: %s", fileName );
		shaderBuffer = getFileManager().readDataFromFile( fileName, shaderBufferSize );		

		if( shaderBuffer != nullptr )
		{
			loadShaderFromBuffer( shaderBuffer, shaderType );
		}
		else
		{
			std::stringstream ss;
			ss << "Could not open shader file: " << fileName;
			std::string errorMessage;
			ss >> errorMessage;
			MonkyException::fatalErrorMessageBox( "Shader file open error", errorMessage.c_str() );
			consolePrintf( "Failed to load shader" );
		}
		SAFE_DELETE( shaderBuffer );
	}

	void Shader::loadShaderFromBuffer( const char* shaderBuffer, GLenum shaderType )
	{
		GLuint shaderID = 0;

		if( shaderBuffer != nullptr )
		{
			int shaderBufferSize = std::strlen( shaderBuffer );
			std::string shaderString;
			shaderString.assign( shaderBuffer, shaderBufferSize );
			//shaderString.assign( std::istreambuf_iterator< char >( shaderFile ), std::istreambuf_iterator< char >() );
			shaderID = glCreateShader( shaderType );


			consolePrintf( "Sending shader source to openGL" );
			const GLchar* shaderSource = shaderString.c_str();
			glShaderSource( shaderID, 1, reinterpret_cast< const GLchar** >( &shaderSource ), nullptr );

			consolePrintf( "Compiling shader" );
			glCompileShader( shaderID );

			GLint compiled = GL_FALSE;
			glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compiled );

			consolePrintf( "Getting compile status" );
			if( compiled != GL_TRUE )
			{
				handleShaderCompilerError( shaderID, "", shaderSource );
			}

			consolePrintf( "Shader loaded" );

			m_shaderID = shaderID;
		}
	}

	void Shader::handleShaderCompilerError( GLuint shaderID, const char* fileName, const std::string& shaderBuffer )
	{
		GLint length = 0;
		GLchar* log;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &length );
		log = new char[ length + 1 ];
		glGetShaderInfoLog( shaderID, length, &length, log );
		
		std::string sLog( log );
		std::vector< std::string > errorTokens;
		stringTokenizer( sLog, errorTokens, "\n" );

		std::string popupOutputMessage;
		for( size_t i = 0; i < errorTokens.size() && errorTokens[i].size() > 0; ++i )
		{

			size_t lineIndexStart = errorTokens[i].find( "(" );
			size_t lineIndexEnd = errorTokens[i].find( ")" );

			std::string lineNumber = 
				errorTokens[i].substr( lineIndexStart + 1, lineIndexEnd - lineIndexStart - 1 );

			std::string errorMessage( "Error on line: " + lineNumber );
		
			std::vector< std::string > lineTokens;
			stringTokenizer( shaderBuffer, lineTokens, "\n" );

			int lineNum = atoi( lineNumber.c_str() );
			errorMessage += "\n>";
			if( lineNum != 0 )
			{
				errorMessage += lineTokens[ lineNum - 1 ] + "\n";
			}
			else
			{
				errorMessage += lineTokens[ 0 ] + "\n";
			}

			errorMessage += errorTokens[i];
		
			std::string consoleOutput( "./" );
			consoleOutput += fileName;
			consoleOutput += "(" + lineNumber + "): " + errorTokens[i] + "\n";

			//MonkyException::printToCompilerOutputConsole( consoleOutput.c_str() );
			popupOutputMessage += errorMessage + "\n\n";
		}
		MonkyException::printToCompilerOutputConsole( sLog.c_str() );

		//MonkyException::fatalErrorMessageBox( "Shader Error", popupOutputMessage.c_str() );
	}

	std::map< std::string, Shader* > Shader::sm_shaders;
}
