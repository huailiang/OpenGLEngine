#version 330 core

#define _Light_

out vec4 FragColor;

in vec2 TexCoords;

#ifdef _Light_

#include "lib/phong.glsl"

#endif // _Light_


uniform sampler2D texture_diffuse1;

void main()
{
#ifdef _Light_
    vec3 litColor = LightColor();
    FragColor = FragColor * vec4(litColor,1);
#else
    FragColor = texture(texture_diffuse1, TexCoords);
#endif
}
