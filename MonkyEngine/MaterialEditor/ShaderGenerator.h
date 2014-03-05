#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/31/2014
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

		enum ShaderOutputChannels
		{
			SOC_DIFFUSE,
			SOC_NORMAL,
			SOC_SPECULAR,
			SOC_EMISSIVE,
			SOC_OPACITY,
			SOC_VERTEX_OFFSET,
			SOC_COUNT
		};

		static const std::string DIFFUSE_CHANNEL_VARIABLE_NAME;
		static const std::string NORMAL_CHANNEL_VARIABLE_NAME;
		static const std::string SPECULAR_CHANNEL_VARIABLE_NAME;
		static const std::string EMISSIVE_CHANNEL_VARIABLE_NAME;
		static const std::string OPACITY_CHANNEL_VARIABLE_NAME;
		static const std::string FRAG_COLOR_OUT_CHANNEL;
		static const std::string VERTEX_OFFSET_CHANNEL_NAME;

		ShaderGenerator( ShaderVersion version );
		virtual ~ShaderGenerator();

		virtual void ResetGenerator();


		bool ProcessNode( XMLParser& parser, XMLNode* node );
		std::string GenerateShader();

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
		
		void EnableOuputChannel( ShaderOutputChannels comp );

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
		std::string GetLightingStructInfo() const;	
		std::string GetOutputChannelVariableDeclarations() const;

		virtual std::string GetShaderOutCode() = 0;
		virtual std::string GetLightingFunctionDefinitions() const = 0;

		void vAddLogMessage( const char* format, Color4f color, va_list args );

		// hashed string of name for map index
		std::map< unsigned int, ShaderVariable > m_uniforms;
		std::map< unsigned int, ShaderVariable > m_inVariables;
		std::map< unsigned int, ShaderVariable > m_outVariables;
		std::map< unsigned int, ShaderVariable > m_variables;
		std::vector< ShaderVariable* > m_orderToDeclareVariables;
		std::vector< ShaderVariable* > m_orderToDeclareInVariables;

		std::string m_statementsString;

		std::map< unsigned int, StatementNodeProcessor* > m_statementNodeProcessors;

		ShaderVersion m_version;

		bool m_outputChannelEnabled[ SOC_COUNT ];

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