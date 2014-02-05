#version 330


#pragma optionNV unroll all
struct Light {
vec4 color;
vec3 direction;
};

const int MAX_NUM_LIGHTS = 8;
uniform Light uLights[ MAX_NUM_LIGHTS ];
float SPECULAR_POWER = 32.0;

uniform float time;
uniform sampler2D RockNormal;
uniform sampler2D RockEmissive;
uniform sampler2D RockSpecular;
uniform sampler2D RockDiffuse;
uniform mat4 uMVPMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform int uUnlit;
uniform mat4 uModelMatrix;
in vec4 vColor;
in vec2 vTexCoord;
in vec4 vScreenPos;
in vec3 vCameraToVertexTBN;
in vec3 vWorldPosition;
in vec3 vVertexNormal;
in vec3 vLightDirTBN[ MAX_NUM_LIGHTS ];
out vec4 fragColor;
vec4 diffuseColor;
vec4 normalColor;
vec4 specularColor;
vec4 emmisiveColor;
float opacity;

vec3 transformColorToNormal()
{
	vec4 color;
	color = normalColor;
	return 2.0*( color.rgb ) - vec3( 1.0 );
}
vec4 getDirectLight( in Light light, in vec3 normal, in vec3 lightDirTBNNormalized ) 
{
	float directIntensity = max( 0.0, dot( normal, -lightDirTBNNormalized ) );
	return vec4( diffuseColor.rgb * directIntensity * light.color.rgb, diffuseColor.a );
}
vec4 getSpecularLight( in vec3 normal, in vec3 lightDirTBNNormalized )
{
	vec3 idealLight = reflect( normalize( vCameraToVertexTBN ), normal );
	float specularPower = pow( dot( idealLight, -lightDirTBNNormalized ), SPECULAR_POWER );
	float specularIntensity = max( 0.0, specularPower );
	return specularIntensity*specularColor;
}
vec4 getADSLighting()
{
	vec4 totalDirect;
	vec4 totalSpecular;
	vec3 normal = normalize( transformColorToNormal() );

	//Loop through all the lights
	for( int i = 0; i < MAX_NUM_LIGHTS; ++i )
	{
		vec3 lightDirTBNNormalized = normalize( vLightDirTBN[i] );
	
		totalDirect += getDirectLight( uLights[i], normal, lightDirTBNNormalized );
		totalSpecular += getSpecularLight( normal, lightDirTBNNormalized );
	}

	vec4 stdDirectLight = clamp( totalDirect, vec4( 0,0,0,0 ), vec4( 1, 1, 1, 1 ) );
	vec4 excessiveDirectLight = totalDirect - stdDirectLight;
	excessiveDirectLight *= .3;
	vec4 actualDirectLight =  stdDirectLight + excessiveDirectLight;
	
	vec4 tempFragColor = vec4( 0, 0, 0, 0.0 );
	
	tempFragColor += actualDirectLight;
	tempFragColor += totalSpecular;
	return tempFragColor;
}
vec4 getColorFromLighting()
{
	vec4 tempFragColor = vec4( 0.0, 0.0, 0.0, 0.0 );
	tempFragColor = getADSLighting();
	tempFragColor += emmisiveColor;
	tempFragColor.a = opacity;
	return tempFragColor;
}
void main()
{
	diffuseColor = texture( RockDiffuse, vTexCoord );
	normalColor = texture( RockNormal, vTexCoord );
	specularColor = texture( RockSpecular, vTexCoord );
	emmisiveColor = texture( RockEmissive, vTexCoord );
	opacity = 0.5;
	fragColor = getColorFromLighting() * ( 1 - uUnlit ) + diffuseColor * uUnlit;
} 