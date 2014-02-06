#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description:
//
//---------------------------------------------
#include "CommonCoreEngineIncludes.h"

#define VT_REAL 0
#define VT_VECTOR_2 1
#define VT_VECTOR_3 2
#define VT_VECTOR_4 3
#define VT_MAT_3 4
#define VT_MAT_4 5
#define VT_TEXTURE_SAMPLE_2D 6
#define VT_INT 7
#define VT_COUNT 8

namespace Monky
{
	typedef short ShaderVariableType;
	//enum ShaderVariableType
	//{
	//	VT_REAL,
	//	VT_VECTOR_2,
	//	VT_VECTOR_3,
	//	VT_VECTOR_4,
	//	VT_MAT_3,
	//	VT_MAT_4,
	//	VT_TEXTURE_SAMPLE_2D,
	//	VT_INT,
	//	VT_COUNT
	//};

	struct ShaderVariable
	{
		ShaderVariable();
		ShaderVariable( ShaderVariableType type, const std::string& name, const std::string& value, bool isConst );
		ShaderVariableType type;
		std::string name;
		std::string value;
		std::string declarationStatment;
		unsigned char isConst;
	};

	ShaderVariableType GetVariableTypeFromString( const std::string& type );
	std::string GetVariableTypeAsString( ShaderVariableType type );
	std::string GetVariableConstructionFromType( ShaderVariableType type, const std::string& value );
	ShaderVariableType GetVariableTypeFromDataString( const std::string& data );
	bool ValidRealNumber( const std::string& valueStr );
}