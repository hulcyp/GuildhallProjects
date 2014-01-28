#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description:
//
//---------------------------------------------
#include "StatementNodeProcessor.h"

namespace Monky
{
	class VariableNodeProcessor : public StatementNodeProcessor
	{
	public:
		VariableNodeProcessor( const std::string& name, ShaderGenerator* generator );
		~VariableNodeProcessor();

		virtual StatementData ProcessAsRoot( XMLParser& parser, XMLNode* node );
		virtual StatementData ProcessAsChild( XMLParser& parser, XMLNode* node );
	};
}