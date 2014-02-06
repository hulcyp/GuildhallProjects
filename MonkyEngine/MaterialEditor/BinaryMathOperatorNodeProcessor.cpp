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
			if( inputs[i].statement != "" )
			{
				statementData.statement += inputs[i].statement;
				if( i + 1 < inputs.size() )
					statementData.statement += m_operatorSymbol;
			}
		}
		statementData.statement += " )";
		statementData.outputType = inputs[0].outputType;
		return statementData;
	}

	bool BinaryMathOperatorNodeProcessor::IsMathOperationValidForInputList( std::vector< StatementData >& inputs )
	{
		bool valid = true;
		ShaderVariableType typeOfFirst = inputs[0].outputType;
		for( unsigned int i = 0; i < inputs.size() && valid; ++i )
		{
			if( inputs[i].outputType != typeOfFirst )
				valid = false;
			//special case for adding a texture sample and a vec4
			if( ( inputs[i].outputType == VT_TEXTURE_SAMPLE_2D && typeOfFirst == VT_VECTOR_4 ) ||
				( typeOfFirst == VT_TEXTURE_SAMPLE_2D && inputs[i].outputType == VT_VECTOR_4 ) ||
				  typeOfFirst == VT_REAL ||
				  inputs[i].outputType == VT_REAL ) 
				valid = true;
		}
		return valid;
	}

}