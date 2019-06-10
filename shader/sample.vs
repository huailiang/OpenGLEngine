#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

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
