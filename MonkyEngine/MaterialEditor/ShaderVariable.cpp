#include "ShaderVariable.h"

namespace Monky
{
	ShaderVariable::ShaderVariable()
		:	type( VT_REAL )
		,	name( "" )
		,	value( "" )
		,	isConst( false )
	{}
	//----------------------------------------------------------------------
	ShaderVariable::ShaderVariable( ShaderVariableType type, const std::string& name, const std::string& value, bool isConst )
		:	type( type )
		,	name( name )
		,	value( value )
		,	isConst( isConst )
	{}

	//----------------------------------------------------------------------
	// global utililty functions
	//----------------------------------------------------------------------
	Monky::ShaderVariableType GetVariableTypeFromString( const std::string& type )
	{
		static std::map< std::string, ShaderVariableType > variableType;
		if( variableType.empty() )
		{
			variableType[ "Real" ] = VT_REAL;
			variableType[ "Vector2" ] = VT_VECTOR_2;
			variableType[ "Vector3" ] = VT_VECTOR_3;
			variableType[ "Vector4" ] = VT_VECTOR_4;
			variableType[ "Color" ] = VT_VECTOR_4;
			variableType[ "Mat3" ] = VT_MAT_3;
			variableType[ "Mat4" ] = VT_MAT_4;
			variableType[ "Texture2D" ] = VT_TEXTURE_SAMPLE_2D;	
			variableType[ "Integer" ] = VT_INT;
		}

		auto iter = variableType.find( type );
		if( iter != variableType.end() )
			return iter->second;
		else
			return VT_COUNT;
	}
	//----------------------------------------------------------------------
	std::string GetVariableTypeAsString( ShaderVariableType type )
	{
		static bool initialized = false;
		static std::map< ShaderVariableType, std::string > variableType;
		if( !initialized )
		{
			variableType[ VT_COUNT ] = "unknown";
			variableType[ VT_REAL ] = "float";
			variableType[ VT_VECTOR_2 ] = "vec2";
			variableType[ VT_VECTOR_3 ] = "vec3";
			variableType[ VT_VECTOR_4 ] = "vec4";
			variableType[ VT_MAT_3 ] = "mat3";
			variableType[ VT_MAT_4 ] = "mat4";
			variableType[ VT_TEXTURE_SAMPLE_2D ] = "sampler2D";		
			variableType[ VT_INT ] = "int";
		}

		auto iter = variableType.find( type );
		if( iter != variableType.end() )
			return iter->second;
		else
			return "";
	}
	//----------------------------------------------------------------------
	std::string GetVariableConstructionFromType( ShaderVariableType type, const std::string& value )
	{
		std::string typeAsString = GetVariableTypeAsString( type );
		std::string variableConstruction;
		int numberOfValuesReq = 0;
		switch( type )
		{
		case VT_REAL:
			numberOfValuesReq = 1;
			break;
		case VT_VECTOR_2:
			numberOfValuesReq = 2;
			break;
		case VT_VECTOR_3:
			numberOfValuesReq = 3;
			break;
		case VT_VECTOR_4:
			numberOfValuesReq = 4;
			break;
		case VT_MAT_3:
			numberOfValuesReq = 9;
			break;
		case VT_MAT_4:
			numberOfValuesReq = 16;
			break;
		}
		std::vector< std::string > tokens;
		stringTokenizer( value, tokens, "," );
		if( (int)tokens.size() == numberOfValuesReq )
		{
			if( numberOfValuesReq == 1 )
				variableConstruction = value;
			else
				variableConstruction = typeAsString + "(" + value + ")";
		}
		else
		{
			consolePrintColorf( "Invalid number of values entered. Req: %d. Entered: %d. Values entered: %s", color::RED, numberOfValuesReq, (int)tokens.size(), value.c_str() );	
			variableConstruction = "";
		}

		return variableConstruction;
	}
	//----------------------------------------------------------------------
	ShaderVariableType GetVariableTypeFromDataString( const std::string& data )
	{
		std::vector< std::string > tokens;
		stringTokenizer( data, tokens, "," );
		ShaderVariableType type = VT_COUNT;
		switch( (int)tokens.size() )
		{
		case 1:
			type = VT_REAL;
			break;
		case 2:
			type = VT_VECTOR_2;
			break;
		case 3:
			type = VT_VECTOR_3;
			break;
		case 4:
			type  = VT_VECTOR_4;
			break;
		}

		return type;
	}
	//----------------------------------------------------------------------
	bool ValidRealNumber( const std::string& valueStr )
	{
		bool valid = false;
		std::string temp = valueStr;
		stripLeadingAndTrailingWhiteSpace( temp );
		float value = 0.0f;
		if( stringToType( temp, &value ) )
			valid = true;
		return valid;
	}
	

}