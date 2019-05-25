#version 330 core

#ifdef  _SpotLight_
#define _PointLight_
#endif

out vec4 FragColor;

#include "lib/util.glsl"
#include "lib/struct.glsl"

in vec2 TexCoords;
in vec4 LightPos;

// sample texture
uniform sampler2D texture1;
//apply shadow
uniform sampler2D shadow;

uniform Light light;

void main()
{
    vec4 color = texture(texture1, TexCoords);
    float shadow = ShadowCalculation(shadow, LightPos);
    color = (1.0 - shadow) * color;
    FragColor = color;
}
