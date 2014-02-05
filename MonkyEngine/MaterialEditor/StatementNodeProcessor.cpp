#include "StatementNodeProcessor.h"
#include "ShaderGenerator.h"

namespace Monky
{
	StatementData::StatementData()
		:	statement( "" )
		,	outputType( VT_COUNT )
	{}
	//----------------------------------------------------------------------
	StatementData::StatementData( const std::string& statement, ShaderVariableType type )
		:	statement( statement )
		,	outputType( type )
	{}
	//----------------------------------------------------------------------
	StatementData::StatementData( const std::string& statement, ShaderVariableType type, const std::string& variableData )
		:	statement( statement )
		,	outputType( type )
		,	variableData( variableData )
	{}
	//----------------------------------------------------------------------
	StatementNodeProcessor::StatementNodeProcessor( const std::string& name, ShaderGenerator* generator )
		:	m_name( name )
		,	m_shaderGenerator( generator )
	{
		m_shaderGenerator->AddStatementNodeProcessor( this );
	}
	//----------------------------------------------------------------------
	StatementNodeProcessor::~StatementNodeProcessor()
	{

	}

	StatementData StatementNodeProcessor::ProcessInputNode( XMLParser& parser, XMLNode* inputNode )
	{
		std::string statement;
		ShaderVariableType type = VT_COUNT;
		std::string inputData = parser.getXMLElementPCDataAsString( inputNode );
		if( inputData.size() > 0 )
		{
			stripLeadingAndTrailingWhiteSpace( inputData );
			if( inputData[0] == '$' )
			{
				std::vector< std::string > tokens;
				stringTokenizer( inputData, tokens, "," );
				if( tokens.size() > 1 )
				{
					if( !m_shaderGenerator->WasCompilerError() && ValidListOfVariables( tokens ) )
					{
						inputData = StripDollarSignsFromListOfVariables( tokens );
						type = GetVariableTypeFromDataString( inputData );
						std::string variableConstruction = GetVariableConstructionFromType( type, inputData );
						if( variableConstruction.size() == 0 )
							m_shaderGenerator->EnableCompilerErrorFlag();
						else
						{
							statement = variableConstruction;
						}
					}
				}
				else
				{
					inputData = inputData.substr( 1, inputData.npos );
					if( m_shaderGenerator->IsTextureSampleDeclared( inputData ) )
					{
						statement = "texture( " + inputData + ", vTexCoord )";
						type = VT_TEXTURE_SAMPLE_2D;
					}
					
					else if( m_shaderGenerator->IsVariableDeclared( inputData ) )
					{
						statement = inputData;
						type = m_shaderGenerator->GetVariableType( inputData );
					}
					else if( IsVariableAccessingComponentOfVariable( inputData ) )
					{
						statement = inputData;
						type = VT_REAL;
					}
					else
					{
						m_shaderGenerator->AddLogMessage( "Variable undefined: %s", color::RED, inputData.c_str() );
						m_shaderGenerator->EnableCompilerErrorFlag();
					}
				}
			}
			else
			{
				type = GetVariableTypeFromDataString( inputData );
				if( type != VT_COUNT && ValidListOfCommaSeparatedVariables( inputData ) )
				{
					inputData = StripDollarSignsFromCommaSeperatedVariables( inputData );
					std::string variableConstruction = GetVariableConstructionFromType( type, inputData );
					if( variableConstruction.size() == 0 )
						m_shaderGenerator->EnableCompilerErrorFlag();
					else
					{
						statement = variableConstruction;
					}
				}
				else
				{
					m_shaderGenerator->AddLogMessage( "Invalid data entered: %s. Missing '$'?", color::RED, inputData.c_str() ); 
					m_shaderGenerator->EnableCompilerErrorFlag();
				}
			}
		}
		else
		{
			m_shaderGenerator->AddLogMessage( "No input data specified for %s channel", color::RED, m_name.c_str() );
			m_shaderGenerator->EnableCompilerErrorFlag();
		}
		return StatementData( statement, type, inputData );
	}

	bool StatementNodeProcessor::ValidListOfVariables( const std::vector< std::string >& data )
	{
		bool invalidData = false;
		for( unsigned int i = 0; i < data.size() && !invalidData; ++i )
		{
			float value = 0.0f;
			std::string temp = data[i];
			if( temp != "" )
			{
				stripLeadingAndTrailingWhiteSpace( temp );

				if( temp[0] == '$' )
				{
					std::string tempWithoutDollarSign = temp.substr( 1, temp.npos );
					if( !m_shaderGenerator->IsVariableDeclared( tempWithoutDollarSign ) )
						invalidData = true;
					if( invalidData && IsVariableAccessingComponentOfVariable( tempWithoutDollarSign ) )
						invalidData = false;
				}
				else
				{
					invalidData = !ValidRealNumber( temp );
				}
			}
			else
			{
				invalidData = true;
			}
		}
		return !invalidData;
	}
	//----------------------------------------------------------------------
	std::string StatementNodeProcessor::StripDollarSignsFromListOfVariables( const std::vector< std::string >& data )
	{
		std::string finalStr;
		for( unsigned int i = 0; i < data.size(); ++i )
		{
			if( data[i] != "" && data[i][0] == '$')
			{
				finalStr += data[i].substr( 1, data[i].npos );
				
			}
			else
			{
				finalStr += data[i];
			}
			if( i + 1 != data.size() )
				finalStr += ",";
		}
		return finalStr;
	}
	//----------------------------------------------------------------------
	std::string StatementNodeProcessor::StripDollarSignsFromCommaSeperatedVariables( const std::string& data )
	{
		std::vector< std::string > tokens;
		stringTokenizer( data, tokens, "," );
		return StripDollarSignsFromListOfVariables( tokens );
	}
	//----------------------------------------------------------------------
	bool StatementNodeProcessor::ValidListOfCommaSeparatedVariables( const std::string& data )
	{
		std::vector< std::string > tokens;
		stringTokenizer( data, tokens, "," );
		return ValidListOfVariables( tokens );
	}
	//----------------------------------------------------------------------
	std::string StatementNodeProcessor::BuildVariableConstructionFromVariableList( const std::vector< std::string >& dataList, ShaderVariableType& type )
	{
		std::string variableConstruction;
		type = (ShaderVariableType)( (int)VT_REAL + (int)( dataList.size() - 1 ) );
		std::string typeAsString = GetVariableTypeAsString( type );
		variableConstruction = typeAsString + "( ";
		for( unsigned int i = 0; i < dataList.size(); ++i )
		{
			variableConstruction += dataList[i];
			if( i + 1 < dataList.size() )
				variableConstruction += ",";
		}
		variableConstruction += " )";
		return variableConstruction;
	}
	//----------------------------------------------------------------------
	bool StatementNodeProcessor::IsVariableAccessingComponentOfVariable( const std::string variable )
	{
		bool isAccessingComponent = false;
		
		std::vector< std::string > tokens;
		std::string tempVar = variable;
		stripLeadingAndTrailingWhiteSpace( tempVar );
		stringTokenizer( tempVar, tokens, "." );
		if( tokens.size() == 2 && tokens[1].size() == 1 && m_shaderGenerator->IsVariableDeclared( tokens[0] ) )
		{			
			ShaderVariableType variableType = m_shaderGenerator->GetVariableType( tokens[0] );
			switch( variableType )
			{
			case VT_VECTOR_2:
				{
					switch( tokens[1][0] )
					{
					case 'x':
					case 'y':
					case 'r':
					case 'g':
						isAccessingComponent = true;
					}
					break;
				}
			case VT_VECTOR_3:
				{
					switch( tokens[1][0] )
					{
					case 'x':
					case 'y':
					case 'z':
					case 'r':
					case 'g':
					case 'b':
						isAccessingComponent = true;
					}
					break;
				}
			case VT_VECTOR_4:
				{
					switch( tokens[1][0] )
					{
					case 'x':
					case 'y':
					case 'z':
					case 'w':
					case 'r':
					case 'g':
					case 'b':
					case 'a':
						isAccessingComponent = true;
					}
					break;
				}
			}

		}
		return isAccessingComponent;
	}
}