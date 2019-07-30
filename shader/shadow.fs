#version 330 core

#include "lib/phong.glsl"
#include "lib/util.glsl"


out vec4 FragColor;

in vec2 texCoords;
in vec4 fragPosLightSpace;

uniform sampler2D diffuseTexture;

#ifdef CASCADES

const int NUM_CASCADES = 3;
in float clipSpacePosZ;
uniform float gCascadeEndClipSpace[NUM_CASCADES];
uniform sampler2D gShadowMap[NUM_CASCADES];

float CalCascadeShadow(int CascadeIndex, vec4 LightSpacePos)
{
    vec3 projCoords = Proj2Coord01(fragPosLightSpace);
    float depth = texture(gShadowMap[CascadeIndex], projCoords.xy).x;
    if (depth < projCoords.z + 0.0001)
        return 0.5;
    else
        return 1.0;
}

#else

uniform sampler2D shadowMap;

#endif

void main()
{
    vec3 color = texture(diffuseTexture, texCoords).rgb;
    vec3 ambient = 0.3 * color;
    vec3 diffuse = Diffuse();
    vec3 specular = Specular();
   
    
#ifdef CASCADES
    
    float shadow = 0.0;
    vec4 indicator = vec4(0.0, 0.0, 0.0, 0.0);
    
    for (int i = 0 ; i < NUM_CASCADES ; i++)
    {
        if (clipSpacePosZ <= gCascadeEndClipSpace[i])
        {
            shadow = CalcShadowFactor(i, LightSpacePos[i]);
            
            if (i == 0)
                indicator = vec4(0.1, 0.0, 0.0, 0.0);
            else if (i == 1)
                indicator = vec4(0.0, 0.1, 0.0, 0.0);
            else if (i == 2)
                indicator = vec4(0.0, 0.0, 0.1, 0.0);
            
            break;
        }
    }
    vec3 lighting = ambient + shadow * (diffuse + specular)) * color + indicator;
    
#else
    
    // calculate shadow
    float shadow = ShadowCalculation(shadowMap, fragPosLightSpace);
    
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
#endif
    
    FragColor = vec4(lighting, 1.0);
}
