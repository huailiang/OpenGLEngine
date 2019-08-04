#version 330 core

#include "lib/phong.glsl"
#include "lib/util.glsl"

out vec4 FragColor;

in vec2 texCoords;
uniform sampler2D diffuseTexture;

#ifdef CASCADES

const int NUM_CASCADES = 3;
in float clipSpacePosZ;
in vec4 LightSpacePos[NUM_CASCADES];
uniform float gCascadeEndClipSpace[NUM_CASCADES];
uniform sampler2D gShadowMap[NUM_CASCADES];

float CalCascadeShadow(int CascadeIndex, vec4 lightspacePos)
{
    vec3 projCoords = Proj2Coord01(lightspacePos);
    float closestDepth = texture(gShadowMap[CascadeIndex], projCoords.xy).x;
    float currDepth = projCoords.z;
    float shadow =  currDepth + 1e-4 > closestDepth ? 1.0 : 0.0;
    ClipShadow(projCoords, shadow);
    return shadow;
}

#else

in vec4 fragPosLightSpace;
uniform sampler2D shadowMap;

#endif

void main()
{
    vec3 color = texture(diffuseTexture, texCoords).rgb;
    vec3 ambient = 0.3 * color;
    vec3 diffuse = Diffuse();
    vec3 specular = Specular();
    
    float shadow = 1.0;
    vec3 indicator = vec3(0.0);

#ifdef CASCADES
    
#ifdef DEBUG
    mat3 arr = mat3(0.1);
#endif
    int find = 0;
    for (int i = 0 ; i < NUM_CASCADES ; i++)
    {
        if (clipSpacePosZ <= gCascadeEndClipSpace[i])
        {
            shadow = CalCascadeShadow(i, LightSpacePos[i]);
#ifdef DEBUG
            indicator = arr[i];
#endif
            find = 1;
            break;
        }
    }
    
    shadow = find * shadow; // if clipSpacePosZ greater than gCascadeEndClipSpace max, will not darw shadow
#else
    
    shadow = ShadowCalculation(shadowMap, fragPosLightSpace);
    
#endif
    
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color + indicator;
    
    FragColor = vec4(lighting, 1.0);
}
