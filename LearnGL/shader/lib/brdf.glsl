#ifndef _BRDF_
#define _BRDF_

/*
 几个BRDF 使用到的核心函数
 */

#include "lib/const.glsl"


/*
 正太分布 NDF
 */
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}


/*
 微面元遮挡函数
 */
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return nom / denom;
}



float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}


/*
 菲涅尔函数
 */
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


/*
  Cook-Torrance BRDF
*/
vec3 BRDF(vec3 normal, vec3 view, vec3 light, float matel, float roughness, vec3 albedo)
{
    vec3 N = normalize(normal);
    vec3 V = normalize(view);
    vec3 L = normalize(light);
    vec3 H = normalize(V+L);
    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, matel);
    
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
    vec3 nominator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    return nominator / max(denominator, 0.001);
}

/*
对应的公式参考
  https://learnopengl.com/PBR/IBL/Diffuse-irradiance
 
 传进来的lightColor 需要算好随着距离衰减（平行光除外）
 */
vec3 PBR(vec3 normal, vec3 view, vec3 light, float matel, float roughness, vec3 albedo, vec3 lightColor)
{
    // Cook-Torrance  BRDF
    vec3 N = normalize(normal);
    vec3 V = normalize(view);
    vec3 L = normalize(light);
    vec3 H = normalize(V + L);
    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, matel);
    
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
    vec3 nominator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001);
    
    // diffuse + specular
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - matel;
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * lightColor * NdotL;
}

#endif
