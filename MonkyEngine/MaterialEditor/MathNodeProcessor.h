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
	class MathNodeProcessor : public StatementNodeProcessor
	{
	public:

		MathNodeProcessor( const std::string& name, ShaderGenerator* generator, int minNumberInputs, int maxNumberInputs );
		virtual ~MathNodeProcessor();

		virtual StatementData ProcessAsRoot( XMLParser& parser, XMLNode* node );
		virtual StatementData ProcessAsChild( XMLParser& parser, XMLNode* node );

		virtual StatementData GenerateMathOperation( XMLParser& parser, XMLNode* root, const std::vector< StatementData >& inputs ) = 0;
		virtual bool IsMathOperationValidForInputList( std::vector< StatementData >& inputs ) = 0;
	private:
		int m_minNumberInputs;
		int m_maxNumberInputs;
		std::string m_commaSeparatedValidAttributes;
	};
}