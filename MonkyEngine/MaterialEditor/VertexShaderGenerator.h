#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 2/4/2014
//	Description:
//
//---------------------------------------------
#include "ShaderGenerator.h"

namespace Monky
{
	class VertexShaderGenerator : public ShaderGenerator
	{
	public:
		VertexShaderGenerator( ShaderVersion version );
		virtual ~VertexShaderGenerator();

		virtual void ResetGenerator();


	protected:
		virtual std::string GetShaderOutCode();
		virtual std::string GetLightingFunctionDefinitions() const;
	};
}