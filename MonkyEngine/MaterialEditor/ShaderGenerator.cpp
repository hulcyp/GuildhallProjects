#include "ShaderGenerator.h"
#include "StringUtils.h"
#include "StatementNodeProcessor.h"
#include "StdLibraryIncludes.h"
//Note TODO: Make it so that you cannot specify a texture as a variable... that's a no no :)

namespace Monky
{	
	const std::string ShaderGenerator::DIFFUSE_CHANNEL_VARIABLE_NAME = "diffuseColor";
	const std::string ShaderGenerator::FRAG_COLOR_OUT_CHANNEL = "fragColor";
	//----------------------------------------------------------------------
	ShaderGenerator::ShaderGenerator( ShaderVersion version )
		:	m_version( version )
		,	m_wasCompilerError( false )
	{
		AddOutVariable( "Vector4", FRAG_COLOR_OUT_CHANNEL );
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
		m_statements.clear();
		m_orderToDeclareVariables.clear();
		AddOutVariable( "Vector4", FRAG_COLOR_OUT_CHANNEL );
		m_wasCompilerError = false;
		for( auto iter = m_statementNodeProcessors.begin(); iter != m_statementNodeProcessors.end(); ++iter )
		{
			iter->second->ReloadProcessorDataToShaderGenerator();
		}
		m_logMessages.clear();
	}
	//----------------------------------------------------------------------
	std::string ShaderGenerator::GenerateShader( XMLParser& parser, XMLNode* root )
	{
		std::string shaderCode;
		
		if( parser.validateXMLChildElements( root, "", m_commaSeparatedValidNodes.c_str() ) )
		{
			XMLNode* node = nullptr;
			for( node = root->FirstChildElement(); node != nullptr; node = node->NextSiblingElement() )
			{
				std::string name = node->Name();
				StatementNodeProcessor* processor = GetStatementNodeProcessor( name );
				if( processor != nullptr )
				{
					std::string statement = processor->ProcessAsRoot( parser, node ).statement;
					if( statement != "" )
						m_statements.push_back( statement );
				}
			}
		}
		else
		{
			//Invalid node in doc...handle this some how lol
		}

		//Compile shader code
		shaderCode += GetShaderVersionCode() + '\n';

		for( auto iter = m_uniforms.begin(); iter != m_uniforms.end(); ++iter )
		{
			shaderCode += iter->second.declarationStatment + "\n";
		}

		for( auto iter = m_inVariables.begin(); iter != m_inVariables.end(); ++iter )
		{
			shaderCode += iter->second.declarationStatment + "\n";
		}

		for( auto iter = m_outVariables.begin(); iter != m_outVariables.end(); ++iter )
		{
			shaderCode += iter->second.declarationStatment + "\n";
		}

		shaderCode += GetMainFunctionStart();
		for( unsigned int i = 0; i < m_orderToDeclareVariables.size(); ++i )
		{
			shaderCode += m_orderToDeclareVariables[i]->declarationStatment + "\n";
		}	

		for( unsigned int i = 0; i < m_statements.size(); ++i )
		{
			shaderCode += m_statements[i] + "\n";
		}

		shaderCode += GetFragColorOutCode();
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
			versionCode[ SV_GLSL_330 ] = "#version 330";			
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
	std::string ShaderGenerator::GetFragColorOutCode()
	{
		return FRAG_COLOR_OUT_CHANNEL + " = " + DIFFUSE_CHANNEL_VARIABLE_NAME + ";";
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