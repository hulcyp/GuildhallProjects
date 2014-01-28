#include "MaterialGenerator.h"
#include "ShaderGenerator.h"
#include "Material.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <glew.h>
#include <Gl/GL.h>

#include "OutputChannelNodeProcessor.h"
#include "SampleTextureNodeProcessor.h"
#include "DeclareTextureSampleNodeProcessor.h"
#include "VariableNodeProcessor.h"
#include "BinaryMathOperatorNodeProcessor.h"
#include "MathFuncWithParamNodeProcessor.h"

#include <fstream>

namespace Monky
{
	MaterialGenerator::MaterialGenerator()
	{
		m_fragShaderGenerator = new ShaderGenerator( ShaderGenerator::SV_GLSL_330 );

		new OutputChannelNodeProcessor( "Diffuse", ShaderGenerator::DIFFUSE_CHANNEL_VARIABLE_NAME, m_fragShaderGenerator );
		new SampleTextureNodeProcessor( "SampleTexture", m_fragShaderGenerator );
		new DeclareTextureSampleNodeProcessor( "TextureSample", m_fragShaderGenerator );
		new VariableNodeProcessor( "Variable", m_fragShaderGenerator );
		new VariableNodeProcessor( "Constant", m_fragShaderGenerator );
		new BinaryMathOperatorNodeProcessor( "Add", m_fragShaderGenerator, "+", 2, -1 );
		new BinaryMathOperatorNodeProcessor( "Subtract", m_fragShaderGenerator, "-", 2, -1 );
		new BinaryMathOperatorNodeProcessor( "Multiply", m_fragShaderGenerator, "*", 2, -1 );
		new BinaryMathOperatorNodeProcessor( "Divide", m_fragShaderGenerator, "/", 2, -1 );	

		new MathFuncWithParamNodeProcessor( "Sine", m_fragShaderGenerator, "sin", 1 );
		new MathFuncWithParamNodeProcessor( "Cosine", m_fragShaderGenerator, "cos", 1 );
		new MathFuncWithParamNodeProcessor( "Abs", m_fragShaderGenerator, "abs", 1 );
		new MathFuncWithParamNodeProcessor( "Ceil", m_fragShaderGenerator, "ceil", 1 );
		new MathFuncWithParamNodeProcessor( "Floor", m_fragShaderGenerator, "floor", 1 );
		new MathFuncWithParamNodeProcessor( "Frac", m_fragShaderGenerator, "fract", 1 );
		new MathFuncWithParamNodeProcessor( "Normalize", m_fragShaderGenerator, "normalize", 1 );
		new MathFuncWithParamNodeProcessor( "SquareRoot", m_fragShaderGenerator, "sqrt", 1 );

		new MathFuncWithParamNodeProcessor( "DotProd", m_fragShaderGenerator, "dot", 2 );
		new MathFuncWithParamNodeProcessor( "CrossProd", m_fragShaderGenerator, "cross", 2 );
		new MathFuncWithParamNodeProcessor( "Fmod", m_fragShaderGenerator, "mod", 2 );
		new MathFuncWithParamNodeProcessor( "Power", m_fragShaderGenerator, "pow", 2 );

		new MathFuncWithParamNodeProcessor( "LinearInterpolate", m_fragShaderGenerator, "smoothstep", 3 );
	}

	MaterialGenerator::~MaterialGenerator()
	{
		SAFE_DELETE( m_fragShaderGenerator );
	}

	Material* MaterialGenerator::GenerateMaterial( const std::string& materialFilePath )
	{
		Material* material = nullptr;
		std::string materialName;
		std::string shaderCode;
		XMLParser parser( materialFilePath.c_str() );
		XMLDocument& doc = parser.getDocument();
		bool failedToLoad = true;
		XMLNode* root = nullptr;
		if( !doc.Error() )
		{
			root = doc.FirstChildElement( "Material" );
			if( root != nullptr )
			{
				if( parser.validateXMLAttributes( root, "name", "" ) )
				{
					m_fragShaderGenerator->AddUniform( "Real", "time" );
					m_fragShaderGenerator->AddUniform( "Mat4", "uProjectionMatrix" );
					m_fragShaderGenerator->AddUniform( "Mat4", "uViewMatrix" );
					m_fragShaderGenerator->AddUniform( "Mat4", "uModelMatrix" );
					m_fragShaderGenerator->AddUniform( "Mat4", "uMVPMatrix" );

					m_fragShaderGenerator->AddInVariable( "Vector4", "vColor" );
					m_fragShaderGenerator->AddInVariable( "Vector2", "vTexCoord0" );

					materialName = parser.getXMLAttributeAsString( root, "name", "" );
					shaderCode = m_fragShaderGenerator->GenerateShader( parser, root );
					m_fragShaderGenerator->OutputShaderLog();
					if( shaderCode != "" && !m_fragShaderGenerator->WasCompilerError() )
						failedToLoad = false;
				}
			}
		}

		if( !failedToLoad )
		{		
			consolePrintColorf( "Successfully loaded material file: %s", color::GREEN, materialFilePath.c_str() );
			std::ofstream outFile( "Test.frag.glsl" );
			outFile.write( shaderCode.c_str(), shaderCode.size() + 1 );
			outFile.close();
			std::string fragShaderName = materialName + ".frag.glsl";
			std::string programName = materialName + ".shaderProgram";
			Shader::createShader( fragShaderName, shaderCode, GL_FRAGMENT_SHADER );
			ShaderProgram::createShaderProgram( programName, "shaders/simpleShader.vertex.glsl", fragShaderName );
			Material::createMaterial( materialName, programName );
			material = Material::getMaterial( materialName );

			XMLNode* textureNode = nullptr;
			for( textureNode = root->FirstChildElement( "TextureSample" ); textureNode != nullptr; textureNode = textureNode->NextSiblingElement( "TextureSample" ) )
			{
				if( parser.validateXMLAttributes( textureNode, "name,textureSampleName", "" ) )
				{
					std::string name = parser.getXMLAttributeAsString( textureNode, "name", "" );
					std::string textureName = parser.getXMLAttributeAsString( textureNode, "textureSampleName", "" );
					if( name != "" && textureName != "" )
					{
						material->addTexture( name.c_str(), Texture::GetTexture( textureName ) );
					}
				}
			}
		}
		else
		{
			consolePrintColorf( "Failed to load material: %s", color::RED, materialFilePath.c_str() );
			material = Material::getMaterial( "SimpleColorMaterial" );
		}
		m_fragShaderGenerator->ResetGenerator();
		return material;
	}
}