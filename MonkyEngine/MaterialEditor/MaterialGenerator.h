#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/16/2014
//	Description:
//
//---------------------------------------------
#include "CommonCoreEngineIncludes.h"

namespace Monky
{
	class Material;
	class ShaderGenerator;

	class MaterialGenerator
	{
	public:
		MaterialGenerator();
		~MaterialGenerator();

		Material* GenerateMaterial( const std::string& materialFilePath );

	private:
		ShaderGenerator* m_fragShaderGenerator;

	};
}