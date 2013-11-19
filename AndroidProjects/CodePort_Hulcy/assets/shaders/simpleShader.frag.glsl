

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

uniform sampler2D uDiffuseMap;
uniform int uUseDiffuseMap;

varying vec4 vColor;
varying vec2 vTexCoord0;


void main()
{
	gl_FragColor = vColor * texture( uDiffuseMap, vTexCoord0 ) * uUseDiffuseMap + vColor * ( 1 - uUseDiffuseMap );
}