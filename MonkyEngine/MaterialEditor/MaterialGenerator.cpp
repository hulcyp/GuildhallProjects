#include "MaterialGenerator.h"
#include "FragmentShaderGenerator.h"
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
#include "VertexShaderGenerator.h"


#include <fstream>

namespace Monky
{
	MaterialGenerator::MaterialGenerator()
	{
		m_fragShaderGenerator = new FragmentShaderGenerator( ShaderGenerator::SV_GLSL_330 );
		m_vertexShaderGenerator = new VertexShaderGenerator( ShaderGenerator::SV_GLSL_330 );

		new OutputChannelNodeProcessor( "Diffuse", ShaderGenerator::DIFFUSE_CHANNEL_VARIABLE_NAME, ShaderGenerator::SOC_DIFFUSE, m_fragShaderGenerator );
		new OutputChannelNodeProcessor( "Normal", ShaderGenerator::NORMAL_CHANNEL_VARIABLE_NAME, ShaderGenerator::SOC_NORMAL, m_fragShaderGenerator );
		new OutputChannelNodeProcessor( "Emissive", ShaderGenerator::EMISSIVE_CHANNEL_VARIABLE_NAME, ShaderGenerator::SOC_EMISSIVE, m_fragShaderGenerator );
		new OutputChannelNodeProcessor( "Specular", ShaderGenerator::SPECULAR_CHANNEL_VARIABLE_NAME, ShaderGenerator::SOC_SPECULAR, m_fragShaderGenerator );
		new OutputChannelNodeProcessor( "Opacity", ShaderGenerator::OPACITY_CHANNEL_VARIABLE_NAME, ShaderGenerator::SOC_OPACITY, m_fragShaderGenerator, VT_REAL );

		new OutputChannelNodeProcessor( "VertexOffset", ShaderGenerator::VERTEX_OFFSET_CHANNEL_NAME, ShaderGenerator::SOC_VERTEX_OFFSET, m_vertexShaderGenerator, VT_VECTOR_3 );
		new VariableNodeProcessor( "Variable", m_vertexShaderGenerator );
		new VariableNodeProcessor( "Constant", m_vertexShaderGenerator );

		
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
		new MathFuncWithParamNodeProcessor( "Reflect", m_fragShaderGenerator, "reflect", 2 );

		new MathFuncWithParamNodeProcessor( "LinearInterpolate", m_fragShaderGenerator, "smoothstep", 3 );
		new MathFuncWithParamNodeProcessor( "Clamp", m_fragShaderGenerator, "clamp", 3 );
	}

	MaterialGenerator::~MaterialGenerator()
	{
		SAFE_DELETE( m_fragShaderGenerator );
	}

	Material* MaterialGenerator::GenerateMaterial( const std::string& materialFilePath )
	{
		Material* material = nullptr;
		std::string materialName;
		std::string fragShaderCode;
		std::string vertShaderCode;
		bool failedToLoad = true;
				
		std::string filePath = "./data/" + materialFilePath;
		FILE* matFile = fopen( filePath.c_str(), "rb" );
		
		
		int graphLayoutBufferSize = 0;
		int versionNumber = 0;
		int pos = 0;
		size_t size = 9;
		if( matFile )
		{
			fread( reinterpret_cast< void* >( &graphLayoutBufferSize ), sizeof( int ), 1, matFile );
		
			fread( reinterpret_cast< void* >( &versionNumber ), sizeof( int ), 1, matFile );
			fseek( matFile, graphLayoutBufferSize + sizeof(int), SEEK_SET );
			pos = ftell( matFile );
			fseek( matFile, 0, SEEK_END );
			size = ftell( matFile ) - pos;
			fclose( matFile );
		}

		matFile = fopen( filePath.c_str(), "rb" );
		
		fseek( matFile, pos, SEEK_SET );
			
		XMLParser parser( matFile, size );
		fclose( matFile );

		XMLDocument& doc = parser.getDocument();
		XMLNode* root = nullptr;
		if( !doc.Error() && versionNumber == MATERIAL_VERSION_NUMBER )
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
					m_fragShaderGenerator->AddUniform( "Integer", "uUnlit" );

					m_vertexShaderGenerator->AddUniform( "Real", "time" );
					m_vertexShaderGenerator->AddUniform( "Mat4", "uProjectionMatrix" );
					m_vertexShaderGenerator->AddUniform( "Mat4", "uViewMatrix" );
					m_vertexShaderGenerator->AddUniform( "Mat4", "uModelMatrix" );
					m_vertexShaderGenerator->AddUniform( "Mat4", "uMVPMatrix" );
					m_vertexShaderGenerator->AddUniform( "Integer", "uUnlit" );

					materialName = parser.getXMLAttributeAsString( root, "name", "" );


					
					
					
					XMLNode* node = nullptr;
					for( node = root->FirstChildElement(); node != nullptr; node = node->NextSiblingElement() )
					{
						if( !m_fragShaderGenerator->ProcessNode( parser, node ) )
						{
							
						}
						if( !m_vertexShaderGenerator->ProcessNode( parser, node ) )
						{
							//Invalid node in document
						}
					}					
					
					vertShaderCode = m_vertexShaderGenerator->GenerateShader();
					fragShaderCode = m_fragShaderGenerator->GenerateShader();
					
					m_fragShaderGenerator->OutputShaderLog();
					m_vertexShaderGenerator->OutputShaderLog();
					if( fragShaderCode != "" && !m_fragShaderGenerator->WasCompilerError() && vertShaderCode != "" && !m_vertexShaderGenerator->WasCompilerError() )
						failedToLoad = false;

					m_vertexShaderGenerator->ResetGenerator();
					m_fragShaderGenerator->ResetGenerator();
				}
			}
		}


		if( !failedToLoad )
		{		
			consolePrintColorf( "Successfully loaded material file: %s", color::GREEN, materialFilePath.c_str() );
			std::ofstream fragFile( "Test.frag.glsl" );
			fragFile.write( fragShaderCode.c_str(), fragShaderCode.size() + 1 );
			fragFile.close();
			std::ofstream vertFile( "Test.vert.glsl" );
			vertFile.write( vertShaderCode.c_str(), vertShaderCode.size() + 1 );
			vertFile.close();

			std::string fragShaderName = materialName + ".frag.glsl";
			std::string vertShaderName = materialName + ".vert.glsl";
			std::string programName = materialName + ".shaderProgram";
			Shader::createShader( fragShaderName, fragShaderCode, GL_FRAGMENT_SHADER );
			Shader::createShader( vertShaderName, vertShaderCode, GL_VERTEX_SHADER );
			ShaderProgram::createShaderProgram( programName, vertShaderName, fragShaderName );
						
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
		return material;
	}
}