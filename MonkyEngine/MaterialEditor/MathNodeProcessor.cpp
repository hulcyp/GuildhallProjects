#include "MathNodeProcessor.h"
#include "ShaderGenerator.h"

namespace Monky
{
	MathNodeProcessor::MathNodeProcessor( const std::string& name, ShaderGenerator* generator, int minNumberInputs, int maxNumberInputs )
		:	StatementNodeProcessor( name, generator )
		,	m_minNumberInputs( minNumberInputs )
		,	m_maxNumberInputs( maxNumberInputs )
		,	m_commaSeparatedValidAttributes( "outputVar" )
	{}

	MathNodeProcessor::~MathNodeProcessor()
	{

	}

	StatementData MathNodeProcessor::ProcessAsRoot( XMLParser& parser, XMLNode* node )
	{
		StatementData statementData;
		if( parser.validateXMLAttributes( node, "", m_commaSeparatedValidAttributes.c_str() ) )
		{
			std::string outputVar = parser.getXMLAttributeAsString( node, "outputVar", "" );
			if( outputVar != "" && m_shaderGenerator->IsVariableDeclared( outputVar ) )
			{
				ShaderVariableType outputVarType = m_shaderGenerator->GetVariableType( outputVar );
				statementData.outputType = outputVarType;
				std::vector< StatementData > inputs;
				XMLNode* child =  nullptr;
				for( child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement() )
				{
					std::string name = child->Name();
					if( name.compare( "Input" ) == 0 )
					{
						inputs.push_back( ProcessInputNode( parser, child ) );
					}
					else
					{
						StatementNodeProcessor* processor = m_shaderGenerator->GetStatementNodeProcessor( name );
						if( processor != nullptr )
						{
							inputs.push_back( processor->ProcessAsChild( parser, child ) );
						}
						else
						{
							consolePrintColorf( "Invalid node: %s", color::RED, name.c_str() );
							m_shaderGenerator->EnableCompilerErrorFlag();
						}
					}
				}
				if( (int)inputs.size() >= m_minNumberInputs && ( m_maxNumberInputs == -1 || (int)inputs.size() <= m_maxNumberInputs  ) )
				{
					bool allSameType = true;
					ShaderVariableType typeOfFirst = inputs[0].outputType;
					for( unsigned int i = 0; i < inputs.size() && allSameType; ++i )
					{
						if( inputs[i].outputType != typeOfFirst )
							allSameType = false;
						//special case for adding a texture sample and a vec4
						if( ( inputs[i].outputType == VT_TEXTURE_SAMPLE_2D && typeOfFirst == VT_VECTOR_4 ) ||
							( typeOfFirst == VT_TEXTURE_SAMPLE_2D && inputs[i].outputType == VT_VECTOR_4 ) )
							allSameType = true;
					}

					if( allSameType )
					{
						StatementData generateMathOpData = GenerateMathOperation( parser, node, inputs );
						if( generateMathOpData.outputType == outputVarType )
							statementData.statement = outputVar + " = " + generateMathOpData.statement + ";";
						else
						{
							consolePrintColorf( "Type mismatch between outputVar and math operation: outputVar: %s mathop: %s", color::RED, outputVar.c_str(), m_name.c_str() );
							m_shaderGenerator->EnableCompilerErrorFlag();
						}
					}
					else
					{
						consolePrintColorf(  "Not all inputs are the same type for math operation: %s", color::RED, m_name.c_str() );
						m_shaderGenerator->EnableCompilerErrorFlag();
					}
				}
				else
				{
					consolePrintColorf( "Invalid number of inputs for math operation. Min number of inputs: %d Max number of inputs: %d", color::RED, m_minNumberInputs, m_maxNumberInputs );
					m_shaderGenerator->EnableCompilerErrorFlag();
				}
			}
			else
			{
				consolePrintColorf( "OutputVar missing or undefined: %s", color::RED, outputVar.c_str() );
			}
		}
		else
		{
			m_shaderGenerator->EnableCompilerErrorFlag();
		}

		return statementData;
	}

	StatementData MathNodeProcessor::ProcessAsChild( XMLParser& parser, XMLNode* node )
	{
		StatementData statementData;
		if( parser.validateXMLAttributes( node, "", m_commaSeparatedValidAttributes.c_str() ) )
		{
			std::vector< StatementData > inputs;
			XMLNode* child =  nullptr;
			for( child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement() )
			{
				std::string name = child->Name();
				if( name.compare( "Input" ) == 0 )
				{
					inputs.push_back( ProcessInputNode( parser, child ) );
				}
				else
				{
					StatementNodeProcessor* processor = m_shaderGenerator->GetStatementNodeProcessor( name );
					if( processor != nullptr )
					{
						inputs.push_back( processor->ProcessAsChild( parser, child ) );
					}
					else
					{
						consolePrintColorf( "Invalid node: %s", color::RED, name.c_str() );
						m_shaderGenerator->EnableCompilerErrorFlag();
					}
				}
			}
			if( (int)inputs.size() >= m_minNumberInputs && ( m_maxNumberInputs == -1 || (int)inputs.size() <= m_maxNumberInputs  ) )
			{

				bool allSameType = true;
				ShaderVariableType typeOfFirst = inputs[0].outputType;
				for( unsigned int i = 0; i < inputs.size() && allSameType; ++i )
				{
					if( inputs[i].outputType != typeOfFirst )
						allSameType = false;
					//special case for adding a texture sample and a vec4
					if( ( inputs[i].outputType == VT_TEXTURE_SAMPLE_2D && typeOfFirst == VT_VECTOR_4 ) ||
						( typeOfFirst == VT_TEXTURE_SAMPLE_2D && inputs[i].outputType == VT_VECTOR_4 ) )
						allSameType = true;
				}

				if( allSameType )
				{
					StatementData generateMathOpData = GenerateMathOperation( parser, node, inputs );
					statementData.statement = generateMathOpData.statement;
					statementData.outputType = generateMathOpData.outputType;
				}
				else
				{
					consolePrintColorf(  "Not all inputs are the same type for math operation: %s", color::RED, m_name.c_str() );
					m_shaderGenerator->EnableCompilerErrorFlag();
				}	
			}
			else
			{
				consolePrintColorf( "Invalid number of inputs for math operation. Min number of inputs: %d Max number of inputs: %d", color::RED, m_minNumberInputs, m_maxNumberInputs );
				m_shaderGenerator->EnableCompilerErrorFlag();
			}
		}
		else
		{
			m_shaderGenerator->EnableCompilerErrorFlag();
		}

		return statementData;
	}

}