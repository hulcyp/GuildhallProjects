#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/16/2014
//	Description:
//
//---------------------------------------------
#include "CommonCoreEngineIncludes.h"
#include "XMLParser.h"

namespace Monky
{
	enum VariableType
	{
		VT_REAL,
		VT_VECTOR2,
		VT_VECTOR3,
		VT_VECTOR4,
		VT_TEXTURE_SAMPLE,
		VT_COUNT
	};

	struct Variable
	{
		VariableType type;
		std::string name;
		std::string value;
		bool isConst;
	};

	VariableType GetVariableTypeFromString( const std::string& type );

	class FragShaderGenerator
	{
	public:
		enum ShaderVersion
		{
			SV_GLSL_330,
			SV_COUNT
		};

		FragShaderGenerator();
		~FragShaderGenerator();

		std::string GenerateShader();

		void AddUniform( const std::string& type, const std::string& name );
		void AddInVariable( const std::string& type, const std::string& name );
		void AddOutVariable( const std::string& type, const std::string& name );

		void AddVariable( const std::string& type, const std::string& name, bool isConst = false );
		void AddTextureSample( const std::string& name, const std::string& textureName );

		void AddStatement( XMLParser& parser, XMLNode* statementRoot );
				
		bool IsValidNode( XMLParser& parser, XMLNode* node );

	protected:
		std::string ProcessStatement( XMLParser& parser, XMLNode* statementRoot );
		std::string ProcessChildStatement( XMLParser& parser, XMLNode* childNode );


		bool IsNodeAnOutputChannel( XMLParser& parser, XMLNode* node );
		std::string ProcessOutputChannelNode( XMLParser& parser, XMLNode* ndoe );

		bool IsNodeAMathOperationNode( XMLParser& parser, XMLNode* node );
		std::string ProcessMathOperationNode( XMLParser& parser, XMLNode* node );

		std::string ProcessTextureSample( XMLParser& parser, XMLNode* node );

		bool IsVariableDeclared( const std::string& variableName );
		bool IsVariableConstant( const std::string& variableName );
		bool IsTextureSampleDeclared( const std::string& constantName );		

	private:
		//hashed string of name for map index
		std::map< unsigned int, Variable > m_uniforms;
		std::map< unsigned int, Variable > m_inVariables;
		std::map< unsigned int, Variable > m_outVariables;
		std::map< unsigned int, Variable > m_variables;
		std::map< unsigned int, std::string > m_textureSamples;

		std::vector< std::string > m_statements;

		std::string m_commaSeparatedOutputChannels;
		std::string m_commaSeparatedValidMathNodes;
		std::string m_commaSeparatedValidVariableNodes;
	};
}