#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;


out vec2 TexCoords;
out vec3 normal;
out vec3 worldPos;

#include "lib/camera.glsl"

uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = Engine_MVP(model) * vec4(aPos, 1.0);
    worldPos = (model * vec4(aPos, 1.0f)).xyz;
    normal = mat3(transpose(inverse(model))) * aNormal;
}
