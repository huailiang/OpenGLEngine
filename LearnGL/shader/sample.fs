#version 330 core

out vec4 FragColor;

#include "lib/util.glsl"

in vec2 TexCoords;
in vec4 LightPos;

// sample texture
uniform sampler2D texture1;
//apply shadow
uniform sampler2D shadow;


void main()
{
    vec4 color = texture(texture1, TexCoords);
    float shadow = ShadowCalculation(shadow, LightPos);
    color = (1.0 - shadow) * color;
    FragColor = color;
}
