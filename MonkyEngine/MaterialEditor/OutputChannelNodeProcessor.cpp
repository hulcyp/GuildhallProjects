#include "OutputChannelNodeProcessor.h"
#include "ShaderGenerator.h"

namespace Monky
{
	OutputChannelNodeProcessor::OutputChannelNodeProcessor( const std::string& name, const std::string& outputVariable, ShaderGenerator* generator )
		:	StatementNodeProcessor( name, generator )
		,	m_outputVariable( outputVariable )
	{
		ReloadProcessorDataToShaderGenerator();
	}
	//----------------------------------------------------------------------
	OutputChannelNodeProcessor::~OutputChannelNodeProcessor()
	{}
	//----------------------------------------------------------------------
	StatementData OutputChannelNodeProcessor::ProcessAsRoot( XMLParser& parser, XMLNode* node )
	{
		std::string statement;
	
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
					if( statementData.outputType == VT_VECTOR_4 || statementData.outputType == VT_TEXTURE_SAMPLE_2D )
					{
						statement = m_outputVariable + " = " + statementData.statement + ";";
					}
					else
					{
						consolePrintColorf( "Incorrect data type: \'%s\' provided for output channel %s", color::RED, GetVariableTypeAsString( statementData.outputType ).c_str(), m_name.c_str() );
						m_shaderGenerator->EnableCompilerErrorFlag();
					}
				}
				else
				{
					StatementNodeProcessor* processor = m_shaderGenerator->GetStatementNodeProcessor( name );
					if( processor != nullptr )
					{
						statement = m_outputVariable + " = " + processor->ProcessAsChild( parser, child ).statement + ";";
					}
					else
					{
						//Invalid node processor
						consolePrintColorf( "Invalid node as child statement for %s output channel: %s", color::RED, name.c_str(), m_name.c_str() );
						m_shaderGenerator->EnableCompilerErrorFlag();
					}
				}
			}
		}
		else
		{
			//Invalid Diffuse statement no input :(
			consolePrintColorf( "Invalid %s channel statement. No input provided", color::RED, m_name.c_str() );
			m_shaderGenerator->EnableCompilerErrorFlag();
		}
		
		StatementData shaderVariable;
		shaderVariable.statement = statement;
		shaderVariable.outputType = VT_VECTOR_4;

		return shaderVariable;
	}
	//----------------------------------------------------------------------
	StatementData OutputChannelNodeProcessor::ProcessAsChild( XMLParser& parser, XMLNode* node )
	{
		return StatementData();
	}

	void OutputChannelNodeProcessor::ReloadProcessorDataToShaderGenerator()
	{
		m_shaderGenerator->AddVariable( "Vector4", m_outputVariable, "vec4(1,1,1,1)", false );

	}

}