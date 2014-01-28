#include "SampleTextureNodeProcessor.h"
#include "ShaderGenerator.h"

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
						if( tokens.size() != 2 )
						{
							consolePrintColorf( "Invalid texture coordinates specified for texture sample: %s", color::RED, textureSample.c_str() );
							m_shaderGenerator->EnableCompilerErrorFlag();
						}
						else
						{
							statement = outputVar + " = texture( " + textureSample + ", vec2( " + texCoords + " ) );";
						}
					}
					else
					{
						statement = outputVar + " = texture( " + textureSample + ", vTexCoord0 );";
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
				std::string texCoords = parser.getXMLElementPCDataAsString( node );
				if( texCoords != "" )
				{
					std::vector< std::string > tokens;
					stringTokenizer( texCoords, tokens, "," );
					if( tokens.size() != 2 )
					{
						consolePrintColorf( "Invalid texture coordinates specified for texture sample: %s", color::RED, textureSample.c_str() );
						m_shaderGenerator->EnableCompilerErrorFlag();
					}
					else
					{
						statement = "texture( " + textureSample + ", vec2( " + texCoords + " ) )";
					}
				}
				else
				{
					statement = "texture( " + textureSample + ", vTexCoord0 )";
				}
			}
		}
		return StatementData( statement, VT_VECTOR_4 );
	}
}