
uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord0;
attribute vec4 aColor;

varying vec4 vColor;
varying vec2 vTexCoord0;

void main()
{
	gl_Position = uMVPMatrix * vec4( aPosition, 1.0 );
	vColor = aColor;//vec4(aColor.x,aColorly,aColor.z,aColor.w);
	vTexCoord0 = aTexCoord0;//vec2( vColor.x, vColor.y );
	//vColor = vec4( aTexCoord0.x, aTexCoord0.y, 0.0, 1.0 );
}