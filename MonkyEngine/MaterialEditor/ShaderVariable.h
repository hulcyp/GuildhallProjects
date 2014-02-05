#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description:
//
//---------------------------------------------
#include "CommonCoreEngineIncludes.h"

namespace Monky
{
	enum ShaderVariableType
	{
		VT_REAL,
		VT_VECTOR_2,
		VT_VECTOR_3,
		VT_VECTOR_4,
		VT_MAT_3,
		VT_MAT_4,
		VT_TEXTURE_SAMPLE_2D,
		VT_INT,
		VT_COUNT
	};

	struct ShaderVariable
	{
		ShaderVariable();
		ShaderVariable( ShaderVariableType type, const std::string& name, const std::string& value, bool isConst );
		ShaderVariableType type;
		std::string name;
		std::string value;
		std::string declarationStatment;
		bool isConst;
	};

	ShaderVariableType GetVariableTypeFromString( const std::string& type );
	std::string GetVariableTypeAsString( ShaderVariableType type );
	std::string GetVariableConstructionFromType( ShaderVariableType type, const std::string& value );
	ShaderVariableType GetVariableTypeFromDataString( const std::string& data );
	bool ValidRealNumber( const std::string& valueStr );
}