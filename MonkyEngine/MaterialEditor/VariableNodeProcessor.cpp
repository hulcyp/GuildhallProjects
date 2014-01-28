#include "VariableNodeProcessor.h"
#include "ShaderGenerator.h"

namespace Monky
{
	VariableNodeProcessor::VariableNodeProcessor( const std::string& name, ShaderGenerator* generator )
		:	StatementNodeProcessor( name, generator )
	{}
	//----------------------------------------------------------------------
	VariableNodeProcessor::~VariableNodeProcessor()
	{}
	//----------------------------------------------------------------------
	StatementData VariableNodeProcessor::ProcessAsRoot( XMLParser& parser, XMLNode* node )
	{
		std::string variableName;
		std::string variableValue;
		std::string variableType;

		if( parser.validateXMLAttributes( node, "name,type", "" ) )
		{		
			variableName = parser.getXMLAttributeAsString( node, "name", "" );
			variableType = parser.getXMLAttributeAsString( node, "type", "" );
			std::string variableData = parser.getXMLElementPCDataAsString( node );
			if( variableType.compare( "Texture2D" ) == 0 )
			{
				consolePrintColorf( "Cannot declare textures as variables", color::RED );
				m_shaderGenerator->EnableCompilerErrorFlag();
			}
			else
			{

				XMLNode* child = node->FirstChildElement();
				if( child != nullptr )
				{
					for( ; child != nullptr; child = child->NextSiblingElement() )
					{
						std::string name = child->Name();
						if( name.compare( "Input" ) == 0 )
						{
							StatementData statementData;
							statementData = ProcessInputNode( parser, child );
							if( statementData.outputType != VT_COUNT )
							{
								if( statementData.outputType == VT_TEXTURE_SAMPLE_2D )
								{
									consolePrintColorf( "Cannot declare a variable of type Texture2D", color::RED );
								}
								else
								{
									variableValue = statementData.statement;
								}
							}
							else
							{
								variableValue = "0";
								variableType = "Real";
							}						
						}
						else
						{
							StatementNodeProcessor* processor = m_shaderGenerator->GetStatementNodeProcessor( name );
							if( processor != nullptr )
							{
								variableValue = processor->ProcessAsChild( parser, child ).statement;
							}
							else
							{
								//Invalid node processor
								consolePrintColorf( "Invalid child node: %s for variable: %s", color::RED, name.c_str(), variableName.c_str() );
								m_shaderGenerator->EnableCompilerErrorFlag();
							}
						}
					}
				}
				else
				{
					std::vector< std::string > variableDataList;
					stringTokenizer( variableData, variableDataList, "," );
					if( ValidListOfVariables( variableDataList ) )
						variableValue = GetVariableConstructionFromType( GetVariableTypeFromString( variableType ), StripDollarSignsFromCommaSeperatedVariables( variableData ) );
					else
					{
						for( unsigned int i = 0; i < variableDataList.size(); ++i )
						{
							if( variableDataList[i][0] == '$' )
								variableDataList[i] = variableDataList[i].substr( 1, variableDataList[i].npos );
							else if( !ValidRealNumber( variableDataList[i] ) )
							{
								consolePrintColorf( "Syntax error in variable declaration in variable name usage: %s. Missing \'$\'?", color::RED, variableData.c_str() );
								m_shaderGenerator->EnableCompilerErrorFlag();
							}
						}
						if( !m_shaderGenerator->WasCompilerError() && ValidListOfVariables( variableDataList ) )
						{
							variableValue = StripDollarSignsFromListOfVariables( variableDataList );
						}
						else
						{
							consolePrintColorf( "Invalid data entered for variable declaration: Variable name: %s Data: %s", color::RED, variableName.c_str(), variableData.c_str() );
							m_shaderGenerator->EnableCompilerErrorFlag();
						}
					}
				}
			}
		}

		if( variableName.size() > 0 )
			m_shaderGenerator->AddVariable( variableType, variableName, variableValue, ( m_name.compare( "Constant" ) == 0 ) );
		return StatementData( "", VT_COUNT );
	}
	//----------------------------------------------------------------------
	StatementData VariableNodeProcessor::ProcessAsChild( XMLParser& parser, XMLNode* node )
	{
		return StatementData();
	}
}
