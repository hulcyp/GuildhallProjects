#include "DeclareTextureSampleNodeProcessor.h"
#include "ShaderGenerator.h"

namespace Monky
{
	DeclareTextureSampleNodeProcessor::DeclareTextureSampleNodeProcessor( const std::string& name, ShaderGenerator* generator )
		:	StatementNodeProcessor( name, generator )
	{

	}
	//----------------------------------------------------------------------
	DeclareTextureSampleNodeProcessor::~DeclareTextureSampleNodeProcessor()
	{

	}
	//----------------------------------------------------------------------
	StatementData DeclareTextureSampleNodeProcessor::ProcessAsRoot( XMLParser& parser, XMLNode* node )
	{
		if( parser.validateXMLAttributes( node, "name,textureSampleName","" ) )
		{
			std::string name = parser.getXMLAttributeAsString( node, "name", "" );
			m_shaderGenerator->AddUniform( "Texture2D", name );
		}
		return StatementData();
	}
	//----------------------------------------------------------------------
	StatementData DeclareTextureSampleNodeProcessor::ProcessAsChild( XMLParser& parser, XMLNode* node )
	{
		return StatementData();
	}
}