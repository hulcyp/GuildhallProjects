precision mediump float;

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
	vec4 useDiffuseMap = vec4( uUseDiffuseMap, uUseDiffuseMap, uUseDiffuseMap, uUseDiffuseMap );
	
	//vec2 texCoord = vec2( vColor.r, vColor.g );
	
	gl_FragColor = vColor * texture2D( uDiffuseMap, vTexCoord0 ) * useDiffuseMap + vColor * ( vec4(1,1,1,1) - useDiffuseMap );

}