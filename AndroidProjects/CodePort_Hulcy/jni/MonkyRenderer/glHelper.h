#pragma once
#include "CoreEngine/StdLibraryIncludes.h"
#include "CoreEngine/Assertion.h"

#include <GLES/gl.h>
#include <GLES2/gl2.h>

namespace Monky
{
	std::string& getGlErrorAsString();

#define glAssertError	\
	assertion( glGetError() == GL_NO_ERROR, getGlErrorAsString().c_str() );
}
