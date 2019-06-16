#version 330 core


#ifndef VERT_TYPE
#define VERT_TYPE  0x0111
#endif

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
    int high = boneid / 65025;
    int x = boneid % 65025;
    
    mat4 mskin = bones[high] * weights.x +
                 bones[x/256] * weights.y +
                 bones[x%256] * weights.z;
    
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
