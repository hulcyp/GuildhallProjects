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
	class MathFuncWithParamNodeProcessor : public MathNodeProcessor
	{
	public:
		MathFuncWithParamNodeProcessor( const std::string& name, ShaderGenerator* generator, const std::string& funcName, int numberOfParams );
		virtual ~MathFuncWithParamNodeProcessor();

		StatementData GenerateMathOperation( XMLParser& parser, XMLNode* root, const std::vector< StatementData >& inputs );
		virtual bool IsMathOperationValidForInputList( std::vector< StatementData >& inputs );

	private:
		std::string m_funcName;
	};
}