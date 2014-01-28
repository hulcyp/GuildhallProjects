#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description: This node processer handles adding the code to actual sample a valid texture sample
//
//---------------------------------------------
#include "StatementNodeProcessor.h"

namespace Monky
{
	class SampleTextureNodeProcessor : public StatementNodeProcessor
	{
	public:
		SampleTextureNodeProcessor( const std::string& name, ShaderGenerator* generator );
		virtual ~SampleTextureNodeProcessor();

		virtual StatementData ProcessAsRoot( XMLParser& parser, XMLNode* node );
		virtual StatementData ProcessAsChild( XMLParser& parser, XMLNode* node );
	};
}