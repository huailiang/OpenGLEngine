#version 330 core

#include "lib/appdata.glsl"

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model  * vec4(aPos, 1.0);
}
