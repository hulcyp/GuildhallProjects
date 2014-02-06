#include "ShaderGenerator.h"
#include "StringUtils.h"
#include "StatementNodeProcessor.h"
#include "StdLibraryIncludes.h"
//Note TODO: Make it so that you cannot specify a texture as a variable... that's a no no :)

namespace Monky
{	
	const std::string ShaderGenerator::DIFFUSE_CHANNEL_VARIABLE_NAME = "diffuseColor";
	const std::string ShaderGenerator::NORMAL_CHANNEL_VARIABLE_NAME = "normalColor";
	const std::string ShaderGenerator::SPECULAR_CHANNEL_VARIABLE_NAME = "specularColor";
	const std::string ShaderGenerator::EMISSIVE_CHANNEL_VARIABLE_NAME = "emmisiveColor";
	const std::string ShaderGenerator::OPACITY_CHANNEL_VARIABLE_NAME = "opacity";
	const std::string ShaderGenerator::FRAG_COLOR_OUT_CHANNEL = "fragColor";
	const std::string ShaderGenerator::VERTEX_OFFSET_CHANNEL_NAME = "vertexOffset";

	//----------------------------------------------------------------------
	ShaderGenerator::ShaderGenerator( ShaderVersion version )
		:	m_version( version )
		,	m_wasCompilerError( false )
	{
		//AddOutVariable( "Vector4", FRAG_COLOR_OUT_CHANNEL );
		for( int i = 0; i < SOC_COUNT; ++i )
		{
			m_outputChannelEnabled[i] = false;
		}
	}
	//----------------------------------------------------------------------
	ShaderGenerator::~ShaderGenerator()
	{
		MAP_CLEANUP( m_statementNodeProcessors );
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::ResetGenerator()
	{
		m_uniforms.clear();
		m_inVariables.clear();
		m_outVariables.clear();
		m_variables.clear();
		m_statementsString.clear();
		m_orderToDeclareVariables.clear();
		m_orderToDeclareInVariables.clear();
		m_wasCompilerError = false;
		for( auto iter = m_statementNodeProcessors.begin(); iter != m_statementNodeProcessors.end(); ++iter )
		{
			iter->second->ReloadProcessorDataToShaderGenerator();
		}
		m_logMessages.clear();
		for( int i = 0; i < SOC_COUNT; ++i )
		{
			m_outputChannelEnabled[i] = false;
		}
	}
	//----------------------------------------------------------------------
	bool ShaderGenerator::ProcessNode( XMLParser& parser, XMLNode* node )
	{
		bool success = false;
		std::string name = node->Name();
		StatementNodeProcessor* processor = GetStatementNodeProcessor( name );
		if( processor != nullptr )
		{
			std::string statement = processor->ProcessAsRoot( parser, node ).statement;
			if( statement != "" )
			{
				m_statementsString += "\t" + statement + "\n";
			}
			success = true;
		}
		return success;
	}
	//----------------------------------------------------------------------
	std::string ShaderGenerator::GenerateShader()
	{
		std::string shaderCode;

		//Compile shader code
		shaderCode += GetShaderVersionCode() + '\n';
		shaderCode += GetLightingStructInfo() + '\n';

		for( auto iter = m_uniforms.begin(); iter != m_uniforms.end(); ++iter )
		{
			shaderCode += iter->second.declarationStatment + "\n";
		}

		for( unsigned int i = 0; i < m_orderToDeclareInVariables.size(); ++i )
		{
			shaderCode += m_orderToDeclareInVariables[i]->declarationStatment + "\n";
		}

		for( auto iter = m_outVariables.begin(); iter != m_outVariables.end(); ++iter )
		{
			shaderCode += iter->second.declarationStatment + "\n";
		}

		shaderCode += GetOutputChannelVariableDeclarations();

		shaderCode += GetLightingFunctionDefinitions();

		shaderCode += GetMainFunctionStart();
		for( unsigned int i = 0; i < m_orderToDeclareVariables.size(); ++i )
		{
			shaderCode += "\t" + m_orderToDeclareVariables[i]->declarationStatment + "\n";
		}	

		shaderCode += m_statementsString;

		shaderCode += GetShaderOutCode();
		shaderCode += GetMainFunctionEnd();
		return shaderCode;
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::AddUniform( const std::string& type, std::string name )
	{
		ShaderVariable variable( GetVariableTypeFromString( type ), name, "", false );
		variable.declarationStatment = "uniform " + GetVariableTypeAsString( variable.type ) + " " + name + ";";
		unsigned int hash = HashStringTo32Bytes( name );
		m_uniforms[ hash ] = variable;
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::AddInVariable( const std::string& type, std::string name )
	{
		ShaderVariable variable( GetVariableTypeFromString( type ), name, "", false );
		variable.declarationStatment = "in " + GetVariableTypeAsString( variable.type ) + " " + name + ";";
		unsigned int hash = HashStringTo32Bytes( name );
		m_inVariables[ hash ] = variable;
		m_orderToDeclareInVariables.push_back( &m_inVariables[ hash ] );
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::AddOutVariable( const std::string& type, std::string name )
	{
		ShaderVariable variable( GetVariableTypeFromString( type ), name, "", false );
		variable.declarationStatment = "out " + GetVariableTypeAsString( variable.type ) + " " + name + ";";
		unsigned int hash = HashStringTo32Bytes( name );
		m_outVariables[ hash ] = variable;
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::AddVariable( const std::string& type, const std::string& name, const std::string& value, bool isConst )
	{
		ShaderVariable variable( GetVariableTypeFromString( type ), name, "", false );
		variable.declarationStatment = ( isConst ? "const " : "" ) + GetVariableTypeAsString( variable.type ) + " " + name;
		if( value != "" )
		{
			std::string variableConstruction = value;
			if( variableConstruction.size() == 0 )
				EnableCompilerErrorFlag();
			else
				variable.declarationStatment += " = " + variableConstruction;
		}

		variable.declarationStatment += ";";
		unsigned int hash = HashStringTo32Bytes( name );
		m_variables[ hash ] = variable;
		m_orderToDeclareVariables.push_back( &m_variables[ hash ] );
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::AddStatementNodeProcessor( StatementNodeProcessor* processor )
	{
		unsigned int hash = HashStringTo32Bytes( processor->GetName() );
		StatementNodeProcessor*& newProcessor = m_statementNodeProcessors[ hash ];
		if( newProcessor )
			SAFE_DELETE( newProcessor );
		newProcessor = processor;
		m_commaSeparatedValidNodes += "," + newProcessor->GetName();
	}
	//----------------------------------------------------------------------
	StatementNodeProcessor* ShaderGenerator::GetStatementNodeProcessor( const std::string& name )
	{
		unsigned int hash = HashStringTo32Bytes( name );
		auto iter = m_statementNodeProcessors.find( hash );
		if( iter != m_statementNodeProcessors.end() )
			return iter->second;
		else
			return nullptr;

	}
	//----------------------------------------------------------------------
	bool ShaderGenerator::IsValidNode( XMLNode* node ) const
	{
		unsigned int hash = HashStringTo32Bytes( node->Name() );
		auto iter = m_statementNodeProcessors.find( hash );
		return iter != m_statementNodeProcessors.end();
	}
	//----------------------------------------------------------------------
	bool ShaderGenerator::IsVariableDeclared( const std::string& variableName ) const
	{
		unsigned int hash = HashStringTo32Bytes( variableName );
		auto varIter = m_variables.find( hash );
		auto inIter = m_inVariables.find( hash );
		auto uniformIter = m_uniforms.find( hash );
		bool variableDeclared = false;
		variableDeclared = varIter != m_variables.end() || inIter != m_inVariables.end() || uniformIter != m_uniforms.end();
		return variableDeclared;
	}
	//----------------------------------------------------------------------
	bool ShaderGenerator::IsTextureSampleDeclared( const std::string& textureName ) const
	{
		unsigned int hash = HashStringTo32Bytes( textureName );
		auto iter = m_uniforms.find( hash );
		return iter != m_uniforms.end() && iter->second.type == VT_TEXTURE_SAMPLE_2D;
	}
	//----------------------------------------------------------------------
	bool ShaderGenerator::IsVariableConstant( const std::string& constantName ) const
	{
		unsigned int hash = HashStringTo32Bytes( constantName );
		auto iter = m_variables.find( hash );
		return iter != m_variables.end() && iter->second.isConst;
	}
	ShaderVariableType ShaderGenerator::GetVariableType( const std::string& variableName ) const
	{
		unsigned int hash = HashStringTo32Bytes( variableName );
		auto varIter = m_variables.find( hash );
		auto inIter = m_inVariables.find( hash );
		auto uniformIter = m_uniforms.find( hash );
		ShaderVariableType type = VT_COUNT;
		if( varIter != m_variables.end() )
		{
			type = varIter->second.type;
		}
		else if( inIter != m_inVariables.end() )
		{
			type = inIter->second.type;
		}
		else if( uniformIter != m_uniforms.end() )
		{
			type = uniformIter->second.type;
		}
		return type;
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::EnableOuputChannel( ShaderOutputChannels comp )
	{
		m_outputChannelEnabled[ comp ] = true;
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::AddLogMessage( const char* format, Color4f color, ... )
	{
		va_list args;
		va_start( args, color );
		vAddLogMessage( format, color, args );		
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::AddLogMessage( const char* format, ... )
	{
		va_list args;
		va_start( args, format );
		vAddLogMessage( format, color::WHITE, args );
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::EnableCompilerErrorFlag()
	{
		m_wasCompilerError = true;
	}
	//----------------------------------------------------------------------
	bool ShaderGenerator::WasCompilerError()
	{
		return m_wasCompilerError;
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::OutputShaderLog()
	{
		for( unsigned int i = 0; i < m_logMessages.size(); ++i )
		{
			consolePrintColorf( m_logMessages[i].errorMessage.c_str(), m_logMessages[i].msgColor );
		}
	}
	//----------------------------------------------------------------------
	// Protected member functions
	//----------------------------------------------------------------------
	std::string ShaderGenerator::GetShaderVersionCode()
	{
		static bool initialized = false;
		static std::map< ShaderVersion, std::string > versionCode;
		if( !initialized )
		{
			versionCode[ SV_GLSL_NONE ] = "";
			versionCode[ SV_GLSL_330 ] = "#version 330\n\n";			
		}

		auto iter = versionCode.find( m_version );
		if( iter != versionCode.end() )
			return iter->second;
		else
			return "";
	}
	//----------------------------------------------------------------------
	std::string ShaderGenerator::GetMainFunctionStart()
	{
		return "void main()\n{\n";
	}
	//----------------------------------------------------------------------
	std::string ShaderGenerator::GetMainFunctionEnd()
	{
		return "\n}\0";
	}
	//----------------------------------------------------------------------
	std::string ShaderGenerator::GetLightingStructInfo() const
	{
		static std::string structInfo = "#pragma optionNV unroll all\n" 
			"struct Light {\n" 
			"vec4 color;\n" 
			"vec3 direction;\n};\n\n" 
			"const int MAX_NUM_LIGHTS = 8;\n"
			"uniform Light uLights[ MAX_NUM_LIGHTS ];\n"
			"float SPECULAR_POWER = 32.0;\n";
		//"float FOG_SCALE = 10000.0;\n"
		//"vec4 FOG_COLOR = vec4( 0.0, 0.0, 0.0, 1.0 );\n"
		return structInfo;
	}
	//----------------------------------------------------------------------
	std::string ShaderGenerator::GetOutputChannelVariableDeclarations() const
	{
		static std::vector< std::string > outputChannelVariables;
		if( outputChannelVariables.size() == 0 )
		{
			outputChannelVariables.resize( SOC_COUNT );
			outputChannelVariables[ SOC_DIFFUSE ] = DIFFUSE_CHANNEL_VARIABLE_NAME;
			outputChannelVariables[ SOC_NORMAL ] = NORMAL_CHANNEL_VARIABLE_NAME;
			outputChannelVariables[ SOC_SPECULAR ] = SPECULAR_CHANNEL_VARIABLE_NAME;
			outputChannelVariables[ SOC_EMISSIVE ] = EMISSIVE_CHANNEL_VARIABLE_NAME;
			outputChannelVariables[ SOC_OPACITY ] = OPACITY_CHANNEL_VARIABLE_NAME;
			outputChannelVariables[ SOC_VERTEX_OFFSET ] = VERTEX_OFFSET_CHANNEL_NAME;
		}

		std::string variableDeclaration;
		for( int i = 0; i < SOC_OPACITY; ++i ) 
		{
			if( m_outputChannelEnabled[ i ] )
				variableDeclaration += "vec4 " + outputChannelVariables[ i ] + ";\n";
		}
		if( m_outputChannelEnabled[ SOC_OPACITY ] )
			variableDeclaration += "float " + outputChannelVariables[ SOC_OPACITY ] + ";\n";
		else if( m_outputChannelEnabled[ SOC_VERTEX_OFFSET ] )
			variableDeclaration += "vec3 " + outputChannelVariables[ SOC_VERTEX_OFFSET ] + ";\n";

		variableDeclaration +="\n";
		return variableDeclaration;
	}
	//----------------------------------------------------------------------
	void ShaderGenerator::vAddLogMessage( const char* format, Color4f color, va_list args )
	{
		char text[256];
		vsprintf_s( text, format, args );
		va_end( args );
		LogMessageData msg;
		msg.errorMessage = text;
		msg.msgColor = color;
		m_logMessages.push_back( msg );
	}
}