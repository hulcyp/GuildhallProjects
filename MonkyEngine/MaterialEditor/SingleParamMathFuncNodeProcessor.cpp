#include "SingleParamMathFuncNodeProcessor.h"

namespace Monky
{
	

	SingleParamMathFuncNodeProcessor::SingleParamMathFuncNodeProcessor( const std::string& name, ShaderGenerator* generator, const std::string& funcName )
		:	MathNodeProcessor( name, generator, 1, 1 )
		,	m_funcName( funcName )
	{}
	//----------------------------------------------------------------------
	SingleParamMathFuncNodeProcessor::~SingleParamMathFuncNodeProcessor()
	{}
	//----------------------------------------------------------------------
	Monky::StatementData SingleParamMathFuncNodeProcessor::GenerateMathOperation( XMLParser& parser, XMLNode* root, const std::vector< StatementData >& inputs )
	{
		StatementData statementData;
		statementData.statement = m_funcName + "( " + inputs[0].statement + " )";
		statementData.outputType = inputs[0].outputType;
		return statementData;
	}

}