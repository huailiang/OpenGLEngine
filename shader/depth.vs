#version 330 core

#define VERT_TYPE  0x0001
#include "lib/appdata.glsl"


uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model  * vec4(aPos, 1.0);
}
