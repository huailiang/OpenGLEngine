#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec3 worldPos;
out vec3 normal;
out vec2 texCoords;
out vec4 fragPosLightSpace;

#include "lib/camera.glsl"

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    normal = transpose(inverse(mat3(model))) * aNormal;
    texCoords = aTexCoords;
    fragPosLightSpace = lightSpaceMatrix * vec4(worldPos, 1.0);
    gl_Position = Engine_MVP(model) * vec4(aPos, 1.0);
}
