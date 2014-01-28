#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description: This node processes the Diffuse channel output node
//
//---------------------------------------------
#include "StatementNodeProcessor.h"

namespace Monky
{
	class OutputChannelNodeProcessor : public StatementNodeProcessor
	{
	public:
		OutputChannelNodeProcessor( const std::string& outputVariable, const std::string& name, ShaderGenerator* generator );
		virtual ~OutputChannelNodeProcessor();

		virtual StatementData ProcessAsRoot( XMLParser& parser, XMLNode* node );
		virtual StatementData ProcessAsChild( XMLParser& parser, XMLNode* node );
		virtual void ReloadProcessorDataToShaderGenerator();

	private:
		std::string m_outputVariable;
	};
}