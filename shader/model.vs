#version 330 core

#define VERT_TYPE  0x0111
#include "lib/appdata.glsl"


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
