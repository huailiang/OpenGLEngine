#version 330 core

#define VERT_TYPE  0x0111
#include "lib/appdata.glsl"
#include "lib/brdf.glsl"
#include "lib/camera.glsl"


out vec3 vertColor;
out vec2 texCoord;
out vec3 normal;
out vec3 worldPos;


uniform mat4 model;

void main()
{
    gl_Position = Engine_MVP(model) * vec4(aPos, 1.0f);
    worldPos = (model * vec4(aPos, 1.0f)).xyz;
    normal = mat3(transpose(inverse(model))) * aNormal;
    vertColor = vec3(1.0f,1.0f,1.0f);
    texCoord = vec2(aTexCoords.x, aTexCoords.y);
}
