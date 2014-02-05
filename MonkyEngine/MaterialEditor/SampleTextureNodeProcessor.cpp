#include "SampleTextureNodeProcessor.h"
#include "FragmentShaderGenerator.h"

namespace Monky
{
	SampleTextureNodeProcessor::SampleTextureNodeProcessor( const std::string& name, ShaderGenerator* generator )
		:	StatementNodeProcessor( name, generator )
	{}
	//----------------------------------------------------------------------
	SampleTextureNodeProcessor::~SampleTextureNodeProcessor()
	{}
	//----------------------------------------------------------------------
	StatementData SampleTextureNodeProcessor::ProcessAsRoot( XMLParser& parser, XMLNode* node )
	{
		std::string statement;
		if( parser.validateXMLAttributes( node, "textureSampleName", "outputVar" ) )
		{
			std::string textureSample = parser.getXMLAttributeAsString( node, "textureSampleName", "" );
			std::string outputVar = parser.getXMLAttributeAsString( node, "outputVar", "" );

			if( textureSample != "" )
			{
				if( outputVar != "" )
				{
					std::string texCoords = parser.getXMLElementPCDataAsString( node );
					if( texCoords != "" )
					{
						std::vector< std::string > tokens;
						stringTokenizer( texCoords, tokens, "," );
						if( tokens.size() == 2 )
						{
							std::string texCoordsValues = StripDollarSignsFromListOfVariables( tokens );
							statement = outputVar + " = texture( " + textureSample + ", vec2( " + texCoordsValues + " ) );";
						}
						else if( tokens.size() == 1 && tokens[0] != "" && tokens[0][0] == '$' )
						{
							std::string variable = StripDollarSignsFromListOfVariables( tokens );
							if( m_shaderGenerator->IsVariableDeclared( variable ) && m_shaderGenerator->GetVariableType( variable ) == VT_VECTOR_2 )
								statement = outputVar + " = texture( " + textureSample + ", " + variable + " );";

						}
						else
						{
							m_shaderGenerator->AddLogMessage( "Invalid texture coordinates specified for texture sample: %s", color::RED, textureSample.c_str() );
							m_shaderGenerator->EnableCompilerErrorFlag();
						}
					}
					else
					{
						statement = outputVar + " = texture( " + textureSample + ", vTexCoord );";
					}
				}
			}
		}

		return StatementData( statement, VT_VECTOR_4 );
	}
	//----------------------------------------------------------------------
	StatementData SampleTextureNodeProcessor::ProcessAsChild( XMLParser& parser, XMLNode* node )
	{
		std::string statement;
		if( parser.validateXMLAttributes( node, "textureSampleName", "" ) )
		{
			std::string textureSample = parser.getXMLAttributeAsString( node, "textureSampleName", "" );
			
			if( textureSample != "" )
			{
				if( m_shaderGenerator->IsTextureSampleDeclared( textureSample ) )
				{
					std::string texCoords = parser.getXMLElementPCDataAsString( node );
					if( texCoords != "" )
					{
						std::vector< std::string > tokens;
						stringTokenizer( texCoords, tokens, "," );
						if( tokens.size() == 2 )
						{
							std::string texCoordsValues = StripDollarSignsFromListOfVariables( tokens );
							statement = "texture( " + textureSample + ", vec2( " + texCoordsValues + " ) )";
						}
						else if( tokens.size() == 1 && tokens[0] != "" && tokens[0][0] == '$' )
						{
							std::string variable = StripDollarSignsFromListOfVariables( tokens );
							if( m_shaderGenerator->IsVariableDeclared( variable ) && m_shaderGenerator->GetVariableType( variable ) == VT_VECTOR_2 )
								statement = "texture( " + textureSample + ", " + variable + " )";

						}
						else
						{
							m_shaderGenerator->AddLogMessage( "Invalid texture coordinates specified for texture sample: %s", color::RED, textureSample.c_str() );
							m_shaderGenerator->EnableCompilerErrorFlag();
						}
					}
					else
					{
						statement = "texture( " + textureSample + ", vTexCoord )";
					}
				}
				else
				{
					m_shaderGenerator->AddLogMessage( "Invalid texture sample name: %s", color::RED, textureSample.c_str() );
					m_shaderGenerator->EnableCompilerErrorFlag();
				}
			}
		}
		return StatementData( statement, VT_VECTOR_4 );
	}
}