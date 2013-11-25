

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;
uniform sampler2D uTexels;
uniform float uFontHeight;

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec4 aColor;
attribute vec2 aTexCoord0;
attribute vec3 aTangent;
attribute vec3 aBitangent;

varying vec4 myColor;
varying vec2 texCoord;


void main()
{
	gl_Position = uMVPMatrix * vec4( aPosition, 1 );
	myColor = aColor;
	texCoord = aTexCoord0;
}