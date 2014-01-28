#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/28/2014
//	Description:
//
//---------------------------------------------
#include "MathNodeProcessor.h"

namespace Monky
{
	class SingleParamMathFuncNodeProcessor : public MathNodeProcessor
	{
	public:
		SingleParamMathFuncNodeProcessor( const std::string& name, ShaderGenerator* generator, const std::string& funcName );
		virtual ~SingleParamMathFuncNodeProcessor();

		StatementData GenerateMathOperation( XMLParser& parser, XMLNode* root, const std::vector< StatementData >& inputs );

	private:
		std::string m_funcName;
	};
}