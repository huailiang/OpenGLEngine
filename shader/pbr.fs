#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

#include "ibl/ibl.glsl"
#include "lib/brdf.glsl"
#include "lib/camera.glsl"

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform int debug;
#ifdef IRRADIANCE
// 环境光从irradianceMap中读取
uniform samplerCube irradianceMap;
// used in IBL diffuse
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
#endif

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;
    
    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);
    
    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    
    return normalize(TBN * tangentNormal);
}

void main()
{
    vec3  albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao = texture(aoMap, TexCoords).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(CAMERA_POS - WorldPos);
    vec3 R = reflect(-V, N);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;
        Lo += PBR(N, V, L, metallic, roughness, albedo, radiance);
   }
#ifndef IRRADIANCE
    vec3 ambient = vec3(0.03) * albedo * ao;
#else
    vec3 kS, kD, F0;
    IndirectK(N, V, metallic, albedo, kS, kD, F0);
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;
    
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 F = FresnelSchlickRoughnessIBL(max(dot(N, V), 0.0), F0, roughness);
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
    
    vec3 ambient = (kD * diffuse + specular) * ao;
#endif
    
    vec3 color = ambient + Lo;
    
#ifdef DEBUG
    if(debug == 0)
        color = irradiance;
    else if (debug == 1)
        color = specular;
    else if( debug == 2)
        color = vec3(brdf,0);
    else if (debug == 3)
        color = F;
#endif
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}
