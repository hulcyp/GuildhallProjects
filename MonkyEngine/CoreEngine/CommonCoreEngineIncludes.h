#pragma once

#include "Assertion.h"
#include "CommonTypeDefines.h"
#include "ProfileSystemInterface.h"
#include "EventSystemInterface.h"
#include "StdLibraryIncludes.h"
#include "Logging.h"
#include "Colors.h"
#include "../ConsoleSystem/ConsoleInterface.h"

#define ARRAY_CLEANUP( array, size )	\
	for( int i = 0; i < size; ++i )		\
	{									\
	SAFE_DELETE( array[i] );		\
	}