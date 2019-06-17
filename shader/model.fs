#version 330 core

#define _Light_

out vec4 FragColor;

in vec2 TexCoords;

#ifdef _Light_

#include "lib/phong.glsl"

#endif // _Light_


uniform sampler2D texture_diffuse;

void main()
{
    FragColor = texture(texture_diffuse, TexCoords);

#ifdef _Light_
    vec3 litColor = LightColor();
    FragColor = FragColor * vec4(litColor,1);
#endif
}
