#version 330
#pragma optionNV unroll all

struct Light
{
	vec4 color;
	vec3 direction;
};

const int MAX_NUM_LIGHTS = 8;

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;



uniform Light uLights[ MAX_NUM_LIGHTS ];


layout(location=0)in vec3 aPosition;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec4 aColor;
layout(location=3)in vec2 aTexCoord0;
layout(location=4)in vec3 aTangent;
layout(location=5)in vec3 aBitangent;

out vec4 vColor;
out vec2 vTexCoord;
out vec4 vScreenPos;
out vec3 vCameraToVertexTBN;
out vec3 vWorldPosition;
out vec3 vVertexNormal;

out vec3 vLightDirTBN[ MAX_NUM_LIGHTS ];

void orthoNormalizeTB( out vec3 oTangent, out vec3 oBitangent )
{
	oTangent = aTangent - dot( aNormal, aTangent )*aNormal;
	oBitangent = -aBitangent - dot( aNormal, -aBitangent )*aNormal - 
	( ( dot( oTangent, -aBitangent ) * oTangent ) / dot( aTangent, aTangent ) );
	
	oTangent = normalize( oTangent );
	oBitangent = normalize( oBitangent );
}

mat4 getTBNMatrix()
{
	vec3 orthoTangent;
	vec3 orthoBitangent;
	orthoNormalizeTB( orthoTangent, orthoBitangent );
	return mat4( vec4( orthoTangent, 0.0 ), vec4( orthoBitangent, 0.0 ), vec4( aNormal, 0.0 ), vec4( 0.0, 0.0, 0.0, 1.0 ) );
}


void main()
{
	mat4 inverseTBN = inverse( getTBNMatrix() );
	mat4 invModel = inverse( uModelMatrix );

	for( int i = 0; i < MAX_NUM_LIGHTS; ++i )
	{
		vec4 lightDirTBN = ( inverseTBN * invModel * vec4( normalize( uLights[i].direction ), 0.0 ) );
		vLightDirTBN[i] = lightDirTBN.xyz;
	}
	
	vec4 cameraPosInModel = invModel * vec4( -transpose(mat3(uViewMatrix)) * uViewMatrix[3].xyz, 1.0 );
	vCameraToVertexTBN = normalize( aPosition - cameraPosInModel.xyz ).xyz;
	vCameraToVertexTBN = normalize( inverseTBN * vec4( vCameraToVertexTBN, 0.0 ) ).xyz; 
	

	gl_Position = uMVPMatrix * vec4( aPosition, 1.0 );
	vColor = aColor;
	vTexCoord = aTexCoord0;
	vScreenPos = gl_Position;
	vVertexNormal = aNormal;
}