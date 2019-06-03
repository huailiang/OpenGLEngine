#version 330 core
#define _DirectLight_

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

out vec3 vertColor;
out vec2 texCoord;
out vec3 normal;
out vec3 worldPos;

#ifndef _BRDF_
#define _BRDF_

/*
 几个BRDF 使用到的核心函数
 */

#ifndef _CONST_
#define _CONST_

#define e               2.7182818
#define HALF_MAX        65504.0 // (2 - 2^-10) * 2^15
#define HALF_MAX_MINUS1 65472.0 // (2 - 2^-9) * 2^15
#define EPSILON         1.0e-5

#define PI              3.14159265359
#define TWO_PI          6.28318530718
#define FOUR_PI         12.5663706143
#define INV_PI          0.31830988618
#define INV_TWO_PI      0.15915494309
#define INV_FOUR_PI     0.07957747155
#define HALF_PI         1.57079632679
#define INV_HALF_PI     0.63661977236


#define FLT_EPSILON     1.192092896e-07 // Smallest positive number, such that 1.0 + FLT_EPSILON != 1.0
#define FLT_MIN         1.175494351e-38 // Minimum representable positive floating-point number
#define FLT_MAX         3.402823466e+38 // Maximum representable floating-point number


#ifdef COLORSPACE_GAMMA
#define ColorSpaceGrey fixed4(0.5, 0.5, 0.5, 0.5)
#define ColorSpaceDouble fixed4(2.0, 2.0, 2.0, 2.0)
#define ColorSpaceLuminance vec4(0.22, 0.707, 0.071, 0.0)
#else //line space
#define ColorSpaceGrey fixed4(0.214041144, 0.214041144, 0.214041144, 0.5)
#define ColorSpaceDouble fixed4(4.59479380, 4.59479380, 4.59479380, 2.0)
#define ColorSpaceLuminance vec4(0.0396819152, 0.458021790, 0.00609653955, 1.0)
#endif


#endif



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

#ifndef _CAMERA_
#define _CAMERA_


/*
*
* 使用ubo 也是为了性能优化
* 这里主要存储了主摄像机的数据
*
*/


layout (std140) uniform Block
{
	mat4 CAMERA_PROJ;
	mat4 CAMERA_VIEW;
    vec3 CAMERA_POS;
};


mat4 Engine_VP()
{
	return CAMERA_PROJ * CAMERA_VIEW;
}

mat4 INV_VIEW()
{
    return inverse(CAMERA_VIEW);
}


mat4 INV_PROJ()
{
    return inverse(CAMERA_PROJ);
}


mat4 Engine_MVP(mat4 model)
{
	return Engine_VP() * model;
}


vec3 Camera_WorldPos()
{
    return CAMERA_POS;
}


vec4 Camera_ViewPos()
{
    return CAMERA_VIEW * vec4(CAMERA_POS,1);
}


vec4 Camera_ProjPos()
{
    return Engine_VP() * vec4(CAMERA_POS,1);
}

#endif


uniform mat4 model;

void main()
{
    gl_Position = Engine_MVP(model) * vec4(aPos, 1.0f);
    worldPos = (model * vec4(aPos, 1.0f)).xyz;
    normal = mat3(transpose(inverse(model))) * aNormal;
    vertColor = vec3(1.0f,1.0f,1.0f);
    texCoord = vec2(aTexcoord.x, aTexcoord.y);
}
