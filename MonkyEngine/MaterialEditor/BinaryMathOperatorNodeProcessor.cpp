#include "BinaryMathOperatorNodeProcessor.h"

namespace Monky
{
	

	BinaryMathOperatorNodeProcessor::BinaryMathOperatorNodeProcessor( const std::string& name, ShaderGenerator* generator, const std::string& operatorSymbol, int minNumberInputs, int maxNumberInputs )
		:	MathNodeProcessor( name, generator, minNumberInputs, maxNumberInputs )
		,	m_operatorSymbol( operatorSymbol )
	{}
	//----------------------------------------------------------------------
	BinaryMathOperatorNodeProcessor::~BinaryMathOperatorNodeProcessor()
	{}
	//----------------------------------------------------------------------
	Monky::StatementData BinaryMathOperatorNodeProcessor::GenerateMathOperation( XMLParser& parser, XMLNode* root, const std::vector< StatementData >& inputs )
	{
		StatementData statementData;
		statementData.statement = "( ";
		for( unsigned int i = 0; i < inputs.size(); ++i )
		{
			statementData.statement += inputs[i].statement;
			if( i + 1 < inputs.size() )
				statementData.statement += m_operatorSymbol;
		}
		statementData.statement += " )";
		statementData.outputType = inputs[0].outputType;
		return statementData;
	}

}