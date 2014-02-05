#include "MathFuncWithParamNodeProcessor.h"

namespace Monky
{
	

	MathFuncWithParamNodeProcessor::MathFuncWithParamNodeProcessor( const std::string& name, ShaderGenerator* generator, const std::string& funcName, int numberOfParams )
		:	MathNodeProcessor( name, generator, numberOfParams, numberOfParams )
		,	m_funcName( funcName )
	{}
	//----------------------------------------------------------------------
	MathFuncWithParamNodeProcessor::~MathFuncWithParamNodeProcessor()
	{}
	//----------------------------------------------------------------------
	Monky::StatementData MathFuncWithParamNodeProcessor::GenerateMathOperation( XMLParser& parser, XMLNode* root, const std::vector< StatementData >& inputs )
	{
		StatementData statementData;
		if( inputs.size() > 0 )
		{
			statementData.statement = m_funcName + "( ";
			for( unsigned int i = 0; i < inputs.size(); ++i )
			{
				statementData.statement += inputs[i].statement;
				if( i + 1 != inputs.size() )
					statementData.statement += ",";
			}
			statementData.statement += " )";
			statementData.outputType = inputs[0].outputType;
		}
		return statementData;
	}

	bool MathFuncWithParamNodeProcessor::IsMathOperationValidForInputList( std::vector< StatementData >& inputs )
	{
		bool valid = true;
		ShaderVariableType typeOfFirst = inputs[0].outputType;
		for( unsigned int i = 0; i < inputs.size() && valid; ++i )
		{
			if( inputs[i].outputType != typeOfFirst )
				valid = false;
			//special case for adding a texture sample and a vec4
			if( ( inputs[i].outputType == VT_TEXTURE_SAMPLE_2D && typeOfFirst == VT_VECTOR_4 ) ||
				( typeOfFirst == VT_TEXTURE_SAMPLE_2D && inputs[i].outputType == VT_VECTOR_4 ) )
				valid = true;
		}
		return valid;
	}

}