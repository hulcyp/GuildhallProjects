#version 330
uniform float time;
uniform mat4 uMVPMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform sampler2D RockTexture;
uniform mat4 uModelMatrix;
in vec2 vTexCoord0;
in vec4 vColor;
out vec4 fragColor;
void main()
{
vec4 diffuseColor = vec4(1,1,1,1);
float color = 0.1;
const float PI = 3.141592654;
vec2 color5 = vec2(0.05,0.05);
vec2 xolor2 = ( color5+color5+color5+color5+vec2(0.1,0.1)+color5 );
float green = xolor2.g;
green = sin( floor( time ) );
diffuseColor = ( texture( RockTexture, vTexCoord0 )+vec4(0,green,0,0) );
fragColor = diffuseColor;
} 