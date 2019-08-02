#version 330 core

#include "lib/appdata.glsl"


out vec3 worldPos;
out vec3 normal;
out vec2 texCoords;

#ifdef CASCADES
const int NUM_CASCADES = 3;
out float clipSpacePosZ;
out vec4 LightSpacePos[NUM_CASCADES];
uniform mat4 lightSpaceMatrix[NUM_CASCADES];
#else
uniform mat4 lightSpaceMatrix;
out vec4 fragPosLightSpace;
#endif

#include "lib/camera.glsl"

uniform mat4 model;


void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    normal = transpose(inverse(mat3(model))) * aNormal;
    texCoords = aTexCoords;
    
    gl_Position = Engine_MVP(model) * vec4(aPos, 1.0);

#ifdef CASCADES
    clipSpacePosZ = gl_Position.z;
    
    for (int i=0; i<NUM_CASCADES;i++) {
        LightSpacePos[i] = lightSpaceMatrix[i] * vec4(worldPos, 1.0);
    }
#else
    
    fragPosLightSpace = lightSpaceMatrix * vec4(worldPos, 1.0);
    
#endif
}
