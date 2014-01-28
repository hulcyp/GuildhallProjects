#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description: This node processor handles adding a texture sample to the list of valid textures samples
//
//---------------------------------------------
#include "StatementNodeProcessor.h"

namespace Monky
{
	class DeclareTextureSampleNodeProcessor : public StatementNodeProcessor
	{
	public:
		DeclareTextureSampleNodeProcessor( const std::string& name, ShaderGenerator* generator );
		virtual ~DeclareTextureSampleNodeProcessor();

		virtual StatementData ProcessAsRoot( XMLParser& parser, XMLNode* node );
		virtual StatementData ProcessAsChild( XMLParser& parser, XMLNode* node );
	};
}