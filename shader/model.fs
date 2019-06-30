#version 330 core

#define _Light_

out vec4 FragColor;

in vec2 TexCoords;

#ifdef _Light_

#include "lib/phong.glsl"

#endif // _Light_


uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_specular;
uniform sampler2D texture_ambient;

void main()
{
    FragColor = texture(texture_diffuse, TexCoords);

#ifdef _Light_
    vec3 litColor = LightColor();
    FragColor = FragColor * vec4(litColor,1);
#endif
}
