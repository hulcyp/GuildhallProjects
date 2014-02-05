#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description:
//
//---------------------------------------------
#include "CommonCoreEngineIncludes.h"
#include "XMLParser.h"
#include "ShaderVariable.h"
#include "ShaderGenerator.h"

namespace Monky
{
	class FragmentShaderGenerator : public ShaderGenerator
	{
	public:

		FragmentShaderGenerator( ShaderVersion version );
		virtual ~FragmentShaderGenerator();

		virtual void ResetGenerator();

	protected:			
		virtual std::string GetShaderOutCode();
		virtual std::string GetLightingFunctionDefinitions() const;
	};
}