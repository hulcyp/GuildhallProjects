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
uniform sampler2D LightningTex;
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
vec4 emmisiveColor;

vec3 transformColorToNormal()
{
	vec4 color;
	color = 	vec4( vVertexNormal, 1.0 );
	return color.rgb;
}
vec4 getDirectLight( in Light light, in vec3 normal, in vec3 lightDirTBNNormalized ) 
{
	float directIntensity = max( 0.0, dot( normal, -lightDirTBNNormalized ) );
	return vec4( vColor.rgb * directIntensity * light.color.rgb, vColor.a );
}
vec4 getSpecularLight( in vec3 normal, in vec3 lightDirTBNNormalized )
{
	vec3 idealLight = reflect( normalize( vCameraToVertexTBN ), normal );
	float specularPower = pow( dot( idealLight, -lightDirTBNNormalized ), SPECULAR_POWER );
	float specularIntensity = max( 0.0, specularPower );
	return vec4( 0,0,0,0 );
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
	return tempFragColor;
}
void main()
{
	vec4 GreenColor = vec4(0.172,0.757,0.133,1);
	vec4 MultiFactor = vec4(0,0,0,0);
	vec2 pannerTexCoords0 = vec2(0,0);
	vec2 pannerTexCoords1 = vec2(0,0);
	vec4 colorSample0 = vec4(0,0,0,0);
	vec4 colorSample1 = vec4(0,0,0,0);
	float Brightness = 5;
	vec4 Color = vec4(0.122,0.507,1,1);
	pannerTexCoords0 = ( ( vTexCoord*vec2(1.5,1.5) )+( time*vec2(-1.5,1.0) ) );
	pannerTexCoords1 = ( ( vTexCoord*vec2(3.0,3.0) )+( time*vec2(0.35,0.35) ) );
	colorSample0 = texture( LightningTex, pannerTexCoords0 );
	colorSample1 = texture( LightningTex, pannerTexCoords1 );
	emmisiveColor = ( Color*( ( Brightness*colorSample0.r )+clamp( colorSample1.g,0.0,0.15 ) ) );
	fragColor = getColorFromLighting() * ( 1 - uUnlit ) + vColor * uUnlit;
} 