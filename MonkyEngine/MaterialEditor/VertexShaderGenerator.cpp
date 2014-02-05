#include "VertexShaderGenerator.h"

namespace Monky
{
	
	VertexShaderGenerator::VertexShaderGenerator( ShaderVersion version )
		:	ShaderGenerator( version )
	{
		AddInVariable( "Vector3", "aPosition" );
		AddInVariable( "Vector3", "aNormal" );
		AddInVariable( "Vector4", "aColor" );
		AddInVariable( "Vector2", "aTexCoord0" );
		AddInVariable( "Vector3", "aTangent" );
		AddInVariable( "Vector3", "aBitangent" );


		AddOutVariable( "Vector4", "vColor" );
		AddOutVariable( "Vector2", "vTexCoord" );
		AddOutVariable( "Vector4", "vScreenPos" );
		AddOutVariable( "Vector3", "vCameraToVertexTBN" );
		AddOutVariable( "Vector3", "vWorldPosition" );
		AddOutVariable( "Vector3", "vVertexNormal" );

		AddOutVariable( "Vector3", "vLightDirTBN[ MAX_NUM_LIGHTS ]" );
	}
	//----------------------------------------------------------------------
	VertexShaderGenerator::~VertexShaderGenerator()
	{}
	//----------------------------------------------------------------------
	void VertexShaderGenerator::ResetGenerator()
	{
		ShaderGenerator::ResetGenerator();
		AddInVariable( "Vector3", "aPosition" );
		AddInVariable( "Vector3", "aNormal" );
		AddInVariable( "Vector4", "aColor" );
		AddInVariable( "Vector2", "aTexCoord0" );
		AddInVariable( "Vector3", "aTangent" );
		AddInVariable( "Vector3", "aBitangent" );


		AddOutVariable( "Vector4", "vColor" );
		AddOutVariable( "Vector2", "vTexCoord" );
		AddOutVariable( "Vector4", "vScreenPos" );
		AddOutVariable( "Vector3", "vCameraToVertexTBN" );
		AddOutVariable( "Vector3", "vWorldPosition" );
		AddOutVariable( "Vector3", "vVertexNormal" );

		AddOutVariable( "Vector3", "vLightDirTBN[ MAX_NUM_LIGHTS ]" );
	}
	//----------------------------------------------------------------------	
	std::string VertexShaderGenerator::GetShaderOutCode()
	{
		std::string outputCode =	"	mat4 inverseTBN = inverse( getTBNMatrix() );\n"
									"	mat4 invModel = inverse( uModelMatrix );\n"

									"	for( int i = 0; i < MAX_NUM_LIGHTS; ++i )\n"
									"	{\n"
									"		vec4 lightDirTBN = ( inverseTBN * invModel * vec4( normalize( uLights[i].direction ), 0.0 ) );\n"
									"		vLightDirTBN[i] = lightDirTBN.xyz;\n"
									"	}\n"

									"	vec4 cameraPosInModel = invModel * vec4( -transpose(mat3(uViewMatrix)) * uViewMatrix[3].xyz, 1.0 );\n"
									"	vCameraToVertexTBN = normalize( aPosition - cameraPosInModel.xyz ).xyz;\n"
									"	vCameraToVertexTBN = normalize( inverseTBN * vec4( vCameraToVertexTBN, 0.0 ) ).xyz; \n";

		if( m_outputChannelEnabled[ SOC_VERTEX_OFFSET ] )
			outputCode +=	"	gl_Position = uMVPMatrix * vec4( aPosition + " + VERTEX_OFFSET_CHANNEL_NAME + ", 1.0 );\n";
		else
			outputCode +=	"	gl_Position = uMVPMatrix * vec4( aPosition, 1.0 );\n";
		outputCode +=	"	vColor = aColor;\n"
						"	vTexCoord = aTexCoord0;\n"
						"	vScreenPos = gl_Position;\n"
						"	vVertexNormal = aNormal;\n";

		return outputCode;
	}
	//----------------------------------------------------------------------
	std::string VertexShaderGenerator::GetLightingFunctionDefinitions() const
	{
		const char* orthoNormalizeTBFunc = "void orthoNormalizeTB( out vec3 oTangent, out vec3 oBitangent )\n{\n"
											"	oTangent = aTangent - dot( aNormal, aTangent )*aNormal;\n"
											"	oBitangent = -aBitangent - dot( aNormal, -aBitangent )*aNormal - ( ( dot( oTangent, -aBitangent ) * oTangent ) / dot( aTangent, aTangent ) );\n"
											"	oTangent = normalize( oTangent );\n"
											"	oBitangent = normalize( oBitangent );\n}\n";

		const char* getTBNMatrixFunc = "mat4 getTBNMatrix()\n{\n"
										"	vec3 orthoTangent;\n"
										"	vec3 orthoBitangent;\n"
										"	orthoNormalizeTB( orthoTangent, orthoBitangent );\n"
										"	return mat4( vec4( orthoTangent, 0.0 ), vec4( orthoBitangent, 0.0 ), vec4( aNormal, 0.0 ), vec4( 0.0, 0.0, 0.0, 1.0 ) );\n}\n";



		return std::string( orthoNormalizeTBFunc ) + std::string( getTBNMatrixFunc );
	}


}