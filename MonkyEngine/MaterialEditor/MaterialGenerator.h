#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/16/2014
//	Description:
//
//---------------------------------------------
#include "CommonCoreEngineIncludes.h"

#define MATERIAL_VERSION_NUMBER 1

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
		ShaderGenerator* m_vertexShaderGenerator;
	};
}