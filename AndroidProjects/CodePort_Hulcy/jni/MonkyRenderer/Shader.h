#pragma once

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include "CoreEngine/StdLibraryIncludes.h"

namespace Monky
{
	class Renderer;

	class Shader
	{
	public:
		friend class Renderer;

		Shader( const std::string& fileName, GLenum shaderType );
		~Shader();

		GLuint getShaderID() { return m_shaderID; }

		static Shader* createOrGetShader( const std::string& fileName, GLenum shaderType );
		static void createShader( const std::string& fileName, GLenum shaderType );
		static Shader* getShader( const std::string& fileName );

	protected:
		static void cleanupShaders();
		void loadShaderFromFile( const char* fileName , GLenum shaderType );
		void handleShaderCompilerError( GLuint shaderID, const char* fileName, const std::string& shaderBuffer );
		
	private:
		GLuint m_shaderID;

		static std::map< std::string, Shader* > sm_shaders;
	};
}
