#version 330 core


#include "lib/phong.glsl"
#include "lib/util.glsl"


out vec4 FragColor;

in vec3 vertColor;
in vec2 texCoord;


uniform float scale;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec3 lightcolor = LightColor();
    
    vec4 vc = vec4(vertColor * 1.0, 1.0);
    vec4 tc1 = texture(texture1, texCoord);
    vec4 tc2 = texture(texture2, texCoord);
    vec4 color = vc * mix(tc1, tc2, 0.2f);
    FragColor = vec4(color.rgb * lightcolor, 1);
}
