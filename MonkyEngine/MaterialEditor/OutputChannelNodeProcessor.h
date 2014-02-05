#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description: This node processes the Diffuse channel output node
//
//---------------------------------------------
#include "StatementNodeProcessor.h"
#include "FragmentShaderGenerator.h"
#include "ShaderVariable.h"

namespace Monky
{
	class OutputChannelNodeProcessor : public StatementNodeProcessor
	{
	public:
		OutputChannelNodeProcessor( const std::string& name, const std::string& outputVariable, ShaderGenerator::ShaderOutputChannels comp, ShaderGenerator* generator, ShaderVariableType type = VT_VECTOR_4 );
		virtual ~OutputChannelNodeProcessor();

		virtual StatementData ProcessAsRoot( XMLParser& parser, XMLNode* node );
		virtual StatementData ProcessAsChild( XMLParser& parser, XMLNode* node );
		virtual void ReloadProcessorDataToShaderGenerator();

	private:
		std::string m_outputVariable;
		ShaderGenerator::ShaderOutputChannels m_lightingModelComp;
		ShaderVariableType m_outputType;
	};
}