precision mediump float;

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;
uniform sampler2D uTexels;
uniform float uFontHeight;

varying vec4 myColor;
varying vec2 texCoord;


void main()
{
	gl_FragColor = myColor*texture2D( uTexels, texCoord );
}