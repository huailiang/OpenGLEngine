#version 330 core

#define VERT_TYPE  0x0111
#include "lib/appdata.glsl"


out vec2 TexCoords;
out vec4 LightPos;

uniform mat4 model;
uniform mat4 vp;
uniform mat4 lightSpaceMatrix;

void main()
{
    TexCoords = aTexCoords;
    LightPos = lightSpaceMatrix * model  * vec4(aPos, 1.0);
    gl_Position = vp * model * vec4(aPos, 1.0);
}
