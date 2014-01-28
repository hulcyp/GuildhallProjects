#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/23/2014
//	Description:
//
//---------------------------------------------
#include "MathNodeProcessor.h"

namespace Monky
{
	class BinaryMathOperatorNodeProcessor : public MathNodeProcessor
	{
	public:
		BinaryMathOperatorNodeProcessor( const std::string& name, ShaderGenerator* generator, const std::string& operatorSymbol, int minNumberInputs, int maxNumberInputs );
		virtual ~BinaryMathOperatorNodeProcessor();

		virtual StatementData GenerateMathOperation( XMLParser& parser, XMLNode* root, const std::vector< StatementData >& inputs );

	private:
		std::string m_operatorSymbol;
	};
}