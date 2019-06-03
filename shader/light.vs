#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

out vec3 vertColor;
out vec2 texCoord;
out vec3 normal;
out vec3 worldPos;

#include "lib/brdf.glsl"
#include "lib/camera.glsl"

uniform mat4 model;

void main()
{
    gl_Position = Engine_MVP(model) * vec4(aPos, 1.0f);
    worldPos = (model * vec4(aPos, 1.0f)).xyz;
    normal = mat3(transpose(inverse(model))) * aNormal;
    vertColor = vec3(1.0f,1.0f,1.0f);
    texCoord = vec2(aTexcoord.x, aTexcoord.y);
}
