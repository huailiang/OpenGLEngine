#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

#include "lib/camera.glsl"

uniform mat4 model;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(CAMERA_VIEW * model)));
    vs_out.normal = vec3(CAMERA_PROJ * vec4(normalMatrix * aNormal, 0.0));
    gl_Position = Engine_MVP(model) * vec4(aPos, 1.0);
}
