#version 330 core

#define VERT_TYPE  0x0111
#include "lib/appdata.glsl"
#include "lib/camera.glsl"

out vec2 TexCoords;
out vec3 normal;
out vec3 worldPos;

uniform mat4 model;

mat4 MODEL;

#ifdef ENABLE_SKELETON


uniform mat4 bones[100];

void CalculateSkele()
{
    mat4 mskin = bones[boneid.x] * weight.x +
                 bones[boneid.y] * weight.y +
                 bones[boneid.z] * weight.z;
    
    MODEL = model * mskin;
}

#else


void CalculateSkele()
{
    MODEL = model;
}

#endif


void main()
{
    CalculateSkele();
    TexCoords = aTexCoords;
    gl_Position = Engine_MVP(MODEL) * vec4(aPos, 1.0);
    worldPos = (MODEL * vec4(aPos, 1.0f)).xyz;
    normal = mat3(transpose(inverse(model))) * aNormal;
}
