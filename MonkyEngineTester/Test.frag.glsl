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
vec4 GreenColor = vec4(0.172,0.757,0.133,1);
vec4 MultiFactor = vec4(0,0,0,0);
MultiFactor = ( vec4(10,10,10,10)*( ( 1.5+( 1*cos( ( time*vec4(3,3,3,3) ) ) ) )*GreenColor ) );
diffuseColor = ( texture( RockTexture, vTexCoord0 )*MultiFactor );
fragColor = diffuseColor;
} 