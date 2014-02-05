#include "FragmentShaderGenerator.h"
#include "StringUtils.h"
#include "StatementNodeProcessor.h"
#include "StdLibraryIncludes.h"
//Note TODO: Make it so that you cannot specify a texture as a variable... that's a no no :)

namespace Monky
{	
	//----------------------------------------------------------------------
	FragmentShaderGenerator::FragmentShaderGenerator( ShaderVersion version )
		:	ShaderGenerator( version )
	{
		AddInVariable( "Vector4", "vColor" );
		AddInVariable( "Vector2", "vTexCoord" );
		AddInVariable( "Vector4", "vScreenPos" );
		AddInVariable( "Vector3", "vCameraToVertexTBN" );
		AddInVariable( "Vector3", "vWorldPosition" );
		AddInVariable( "Vector3", "vVertexNormal" );

		AddInVariable( "Vector3", "vLightDirTBN[ MAX_NUM_LIGHTS ]" );

		AddOutVariable( "Vector4", FRAG_COLOR_OUT_CHANNEL );
	}
	//----------------------------------------------------------------------
	FragmentShaderGenerator::~FragmentShaderGenerator()
	{
	}
	//----------------------------------------------------------------------
	void FragmentShaderGenerator::ResetGenerator()
	{
		ShaderGenerator::ResetGenerator();
		AddInVariable( "Vector4", "vColor" );
		AddInVariable( "Vector2", "vTexCoord" );
		AddInVariable( "Vector4", "vScreenPos" );
		AddInVariable( "Vector3", "vCameraToVertexTBN" );
		AddInVariable( "Vector3", "vWorldPosition" );
		AddInVariable( "Vector3", "vVertexNormal" );

		AddInVariable( "Vector3", "vLightDirTBN[ MAX_NUM_LIGHTS ]" );

		AddOutVariable( "Vector4", FRAG_COLOR_OUT_CHANNEL );
	}
	//----------------------------------------------------------------------
	// Protected member functions
	//----------------------------------------------------------------------
	std::string FragmentShaderGenerator::GetShaderOutCode()
	{
		std::string outCode = "\t" + FRAG_COLOR_OUT_CHANNEL + " = " + "getColorFromLighting() * ( 1 - uUnlit ) + ";
		if( m_outputChannelEnabled[ SOC_DIFFUSE ] )
			outCode += DIFFUSE_CHANNEL_VARIABLE_NAME + " * uUnlit;";
		else
			outCode += "vColor * uUnlit;";
		return  outCode;
	}
	//----------------------------------------------------------------------
	std::string FragmentShaderGenerator::GetLightingFunctionDefinitions() const
	{
		std::string normalFunc = "vec3 transformColorToNormal()\n{\n	vec4 color;\n	color = ";
		if( m_outputChannelEnabled[ SOC_NORMAL ] )
		{
			normalFunc += NORMAL_CHANNEL_VARIABLE_NAME + ";\n";
			normalFunc += "	return 2.0*( color.rgb ) - vec3( 1.0 );\n}\n";
		}
		else
		{
			normalFunc += "	vec4( vVertexNormal, 1.0 );\n";
			normalFunc += "	return color.rgb;\n}\n";
		}


		std::string getDirectLightFunc = "vec4 getDirectLight( in Light light, in vec3 normal, in vec3 lightDirTBNNormalized ) \n{\n"
										 "	float directIntensity = max( 0.0, dot( normal, -lightDirTBNNormalized ) );\n";
		
		if( m_outputChannelEnabled[ SOC_DIFFUSE ] )
			getDirectLightFunc += "	return vec4( " + DIFFUSE_CHANNEL_VARIABLE_NAME + ".rgb * directIntensity * light.color.rgb, " + DIFFUSE_CHANNEL_VARIABLE_NAME + ".a );\n}\n";
		else
			getDirectLightFunc += "	return vec4( vColor.rgb * directIntensity * light.color.rgb, vColor.a );\n}\n";


		std::string specularLightFunc = "vec4 getSpecularLight( in vec3 normal, in vec3 lightDirTBNNormalized )\n{\n"
										"	vec3 idealLight = reflect( normalize( vCameraToVertexTBN ), normal );\n"
										"	float specularPower = pow( dot( idealLight, -lightDirTBNNormalized ), SPECULAR_POWER );\n"
										"	float specularIntensity = max( 0.0, specularPower );\n";

		if( m_outputChannelEnabled[ SOC_SPECULAR ] )
			specularLightFunc += "	return specularIntensity*" + SPECULAR_CHANNEL_VARIABLE_NAME + ";\n}\n";
		else
			specularLightFunc += "	return vec4( 0,0,0,0 );\n}\n";
			

		std::string adsLightingFunc =	"vec4 getADSLighting()\n{\n"
										"	vec4 totalDirect;\n"
										"	vec4 totalSpecular;\n"
										"	vec3 normal = normalize( transformColorToNormal() );\n"
										"\n"
										"	//Loop through all the lights\n"
										"	for( int i = 0; i < MAX_NUM_LIGHTS; ++i )\n"
										"	{\n"
										"		vec3 lightDirTBNNormalized = normalize( vLightDirTBN[i] );\n"
										"	\n"
										"		totalDirect += getDirectLight( uLights[i], normal, lightDirTBNNormalized );\n"
										"		totalSpecular += getSpecularLight( normal, lightDirTBNNormalized );\n\t}\n"
										"\n"
										"	vec4 stdDirectLight = clamp( totalDirect, vec4( 0,0,0,0 ), vec4( 1, 1, 1, 1 ) );\n"
										"	vec4 excessiveDirectLight = totalDirect - stdDirectLight;\n"
										"	excessiveDirectLight *= .3;\n"
										"	vec4 actualDirectLight =  stdDirectLight + excessiveDirectLight;\n"
										"	\n"
										"	vec4 tempFragColor = vec4( 0, 0, 0, 0.0 );\n"
										"	\n"
										"	tempFragColor += actualDirectLight;\n"
										"	tempFragColor += totalSpecular;\n"
										"	return tempFragColor;\n}\n";

		std::string getColorFromLightingFunc = "vec4 getColorFromLighting()\n{\n"
												"	vec4 tempFragColor = vec4( 0.0, 0.0, 0.0, 0.0 );\n"
												"	tempFragColor = getADSLighting();\n";

		if( m_outputChannelEnabled[ SOC_EMISSIVE ] )
			getColorFromLightingFunc += "	tempFragColor += " + EMISSIVE_CHANNEL_VARIABLE_NAME + ";\n";

		if( m_outputChannelEnabled[ SOC_OPACITY ] )
			getColorFromLightingFunc += "	tempFragColor.a = " + OPACITY_CHANNEL_VARIABLE_NAME + ";\n";

		getColorFromLightingFunc += "	return tempFragColor;\n}\n";

		return normalFunc + getDirectLightFunc + specularLightFunc + adsLightingFunc + getColorFromLightingFunc;

	}
}