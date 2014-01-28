#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description:
//
//---------------------------------------------
#include "CommonCoreEngineIncludes.h"
#include "XMLParser.h"
#include "ShaderVariable.h"

namespace Monky
{

	
	class StatementNodeProcessor;

	class ShaderGenerator
	{
	public:
		enum ShaderVersion
		{
			SV_GLSL_330,
			SV_GLSL_NONE,
			SV_COUNT
		};

		ShaderGenerator( ShaderVersion version );
		~ShaderGenerator();

		void ResetGenerator();
		std::string GenerateShader( XMLParser& parser, XMLNode* root );

		void AddUniform( const std::string& type, std::string name );
		void AddInVariable( const std::string& type, std::string name );
		void AddOutVariable( const std::string& type, std::string name );

		void AddVariable( const std::string& type, const std::string& name, const std::string& value, bool isConst );
		
		void AddStatementNodeProcessor( StatementNodeProcessor* processor );
		StatementNodeProcessor* GetStatementNodeProcessor( const std::string& name );

		bool IsValidNode( XMLNode* node ) const;


		bool IsVariableDeclared( const std::string& variableName ) const;
		bool IsTextureSampleDeclared( const std::string& textureName ) const;
		bool IsVariableConstant( const std::string& constantName ) const;

		ShaderVariableType GetVariableType( const std::string& variableName ) const;

		static const std::string DIFFUSE_CHANNEL_VARIABLE_NAME;
		static const std::string FRAG_COLOR_OUT_CHANNEL;

		//Logging and error handling
		
		void AddLogMessage( const char* format, Color4f color, ... );
		void AddLogMessage( const char* format, ... );
		void EnableCompilerErrorFlag();
		bool WasCompilerError();
		void OutputShaderLog();

	protected:			
		std::string GetShaderVersionCode();
		std::string GetMainFunctionStart();
		std::string GetMainFunctionEnd();
		std::string GetFragColorOutCode();

	private:
		void vAddLogMessage( const char* format, Color4f color, va_list args );

		// hashed string of name for map index
		std::map< unsigned int, ShaderVariable > m_uniforms;
		std::map< unsigned int, ShaderVariable > m_inVariables;
		std::map< unsigned int, ShaderVariable > m_outVariables;
		std::map< unsigned int, ShaderVariable > m_variables;
		std::vector< ShaderVariable* > m_orderToDeclareVariables;

		std::vector< std::string > m_statements;

		std::string m_commaSeparatedValidNodes;

		std::map< unsigned int, StatementNodeProcessor* > m_statementNodeProcessors;

		ShaderVersion m_version;
		
		//For error handing
		struct LogMessageData
		{
			std::string errorMessage;
			Color4f msgColor;
		};
		
		bool m_wasCompilerError;
		std::vector< LogMessageData > m_logMessages;
	};

}