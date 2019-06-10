#version 330 core
#define VERT_TYPE  0x0111
#include "lib/appdata.glsl"

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

#include "lib/camera.glsl"

uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;
    
    gl_Position =  Engine_MVP(model) * vec4(aPos, 1.0);
}
