#version 330 core
#define _DirectLight_



#ifndef _LIGHT_
#define _LIGHT_

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

#ifndef _STRUCT_
#define _STRUCT_


/*
 light cast material
 */
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


/*
 light inference config
 */
struct Light
{
    vec3 direction;
    vec3 color;
#ifdef  _SpotLight_
#define _PointLight_
    float cutOff;
    float outerCutOff;
#endif
    
#ifdef _PointLight_
    vec3 position;
    vec3 constant;
#endif
};


#endif



/*
 *   冯氏光照模型(Phong Lighting Model)
 */

in vec3 worldPos;
in vec3 normal;

uniform Light light;
uniform Material material;


vec3 Diffuse()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
#ifdef _PointLight_
    lightDir = normalize(light.position - worldPos);
#endif
    float diff = max(dot(norm, lightDir), 0.0f);
    return light.color * diff * material.diffuse;
}


vec3 Specular()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir = normalize(Camera_WorldPos() - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    return light.color * spec * material.specular;
}


vec3 Blinn_Specular()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir = normalize(Camera_WorldPos() - worldPos);
    vec3 halfwayDir = normalize(-lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    return light.color * spec * material.specular;
}


vec3 LightColor()
{
    vec3 ambient = vec3(0.08f) * material.ambient;
    vec3 diffuse = Diffuse();
    vec3 specular = Specular();

#ifdef  _SpotLight_
    vec3 lightDir = normalize(light.position - worldPos);
    float theta = dot(lightDir, normalize(-light.direction));
    //with soft edge
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.25f, 1.0f);
    //without soft edge
    //f/loat intensity = theta < light.cutOff ? 0 : 1;
#endif
    
#ifdef _PointLight_
    float distance =  length(light.position - worldPos);
    vec3 cst = light.constant;
    float atten = 1.0f/(cst.x+cst.y*distance+cst.z*distance*distance);
#endif

#ifdef _DirectLight_
    diffuse *= 2.0;
#endif

    
vec3 lightcolor = ambient + diffuse + specular;
    
#ifdef _PointLight_
    lightcolor *= atten;
#endif
    
#ifdef  _SpotLight_
    lightcolor *= intensity;
#endif

    
    return lightcolor;
}


#endif

#ifndef _UTIL_
#define _UTIL_

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


//saturate is defined in cg/hlsl
#define saturate(x) clamp(x, 0.0, 1.0)

//frac is defined in cg/hlsl
#define frac(x) fract(x)

//lerp is defined in cg/hlsl
#define lerp(a,b,f) mix(a,b,f)

//ddy is defined in cg/hlsl
#define ddy(a) dFdy(a)

//ddx is defined in cg/hlsl
#define ddx(a) dFdx(a)

//atan2 is defined in cg/hlsl
#define atan2(y,x) atan(x,y)


vec2 pow2(vec2 x, float y)
{
    return vec2(pow(x.x, y), pow(x.y,y));
}

vec3 pow3(vec3 x, float y)
{
    return vec3(pow(x.x, y), pow(x.y,y), pow(x.z, y));
}

vec4 pow4(vec4 x, float y)
{
    return vec4(pow(x.x, y), pow(x.y,y), pow(x.z, y), pow(x.w, y));
}

// Unpack normal as DXT5nm (1, y, 1, x) or BC5 (x, y, 0, 1)
// Note neutral texture like "bump" is (0, 0, 1, 1) to work with both plain RGB normal and DXT5nm/BC5
vec3 UnpackNormalmapRGorAG(vec4 packednormal)
{
    packednormal.x *= packednormal.w;
    vec3 normal;
    normal.xy = packednormal.xy * 2.0 - 1.0;
    normal.z = sqrt(1.0 - saturate(dot(packednormal.xy, packednormal.xy)));
    return normal;
}


vec3 UnpackNormalmap(vec4 packednormal)
{
    vec3 normal;
#ifdef _DX5nm_
    normal = UnpackNormalmapRGorAG(packednormal);
#else
    normal.xy = packednormal.xy * 2.0 -1.0;
    normal.z = packednormal.w;
#endif
    return normal;
}


vec4 EncodeFloatRGBA(float v)
{
    vec4 kEncodeMul = vec4(1.0, 255.0, 65025.0, 16581375.0);
    float kEncodeBit = 1.0/255.0;
    vec4 enc = kEncodeMul * v;
    enc = frac(enc);
    enc -= enc.yzww * kEncodeBit;
    return enc;
}


float DecodeFloatRGBA(vec4 enc)
{
    vec4 kDecodeDot = vec4(1.0, 1.0/255.0, 1.0/65025.0, 1.0/16581375.0);
    return dot( enc, kDecodeDot );
}


/*
 灰度公式 区分线性空间和gammaf空间
 */
float Luminance(vec3 rgb)
{
    return dot(rgb, ColorSpaceLuminance.rgb);
}


bool IsGammaSpace()
{
#ifdef COLORSPACE_GAMMA
    return true;
#else
    return false;
#endif
}


float GammaToLinearSpaceExact (float value)
{
    if (value <= 0.04045F)
        return value / 12.92F;
    else if (value < 1.0F)
        return pow((value + 0.055F)/1.055F, 2.4F);
    else
        return pow(value, 2.2F);
}

vec3 GammaToLinearSpace (vec3 sRGB)
{
    // Approximate version from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return sRGB * (sRGB * (sRGB * 0.305306011f + 0.682171111f) + 0.012522878f);
    
    // Precise version, useful for debugging.
    //return vec3(GammaToLinearSpaceExact(sRGB.r), GammaToLinearSpaceExact(sRGB.g), GammaToLinearSpaceExact(sRGB.b));
}

float LinearToGammaSpaceExact (float value)
{
    if (value <= 0.0F)
        return 0.0F;
    else if (value <= 0.0031308F)
        return 12.92F * value;
    else if (value < 1.0F)
        return 1.055F * pow(value, 0.4166667F) - 0.055F;
    else
        return pow(value, 0.45454545F);
}

vec3 LinearToGammaSpace (vec3 linRGB)
{
    linRGB = max(linRGB, vec3(0.f, 0.f, 0.f));
    // An almost-perfect approximation from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return max(1.055f * pow3(linRGB, 0.416666667f) - 0.055f, 0.0f);
    
    // Exact version, useful for debugging.
    //return vec3(LinearToGammaSpaceExact(linRGB.r), LinearToGammaSpaceExact(linRGB.g), LinearToGammaSpaceExact(linRGB.b));
}

// Decodes HDR textures
// handles dLDR, RGBM formats
vec3 DecodeHDR (vec4 data, vec4 decodeInstructions)
{
    // Take into account texture alpha if decodeInstructions.w is true(the alpha value affects the RGB channels)
    float alpha = decodeInstructions.w * (data.a - 1.0) + 1.0;
    
    // If Linear mode is not supported we can skip exponent part
#if defined(COLORSPACE_GAMMA)
    return (decodeInstructions.x * alpha) * data.rgb;
#else
#   if defined(USE_NATIVE_HDR)
    return decodeInstructions.x * data.rgb; // Multiplier for future HDRI relative to absolute conversion.
#   else
    return (decodeInstructions.x * pow(alpha, decodeInstructions.y)) * data.rgb;
#   endif
#endif
}

// required when using a perspective projection matrix
float LinearizeDepth(float depth, float near_plane, float far_plane)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

// 
vec3 Proj2Coord01(vec4 pos)
{
    vec3 projCoords = pos.xyz / pos.w;
    return projCoords * 0.5 + 0.5;
}

// for direct light
float ShadowCalculation(sampler2D shadowmap,  vec4 lightspacePos)
{
    vec3 projCoords = Proj2Coord01(lightspacePos);
    float closestDepth = texture(shadowmap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    if(projCoords.z > 1.0) shadow = 0.0;
    return shadow;
}


#endif



out vec4 FragColor;

in vec3 vertColor;
in vec2 texCoord;


uniform float scale;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec3 lightcolor = LightColor();
    
    vec4 vc = vec4(vertColor * 1.0, 1.0);
    vec4 tc1 = texture(texture1, texCoord);
    vec4 tc2 = texture(texture2, texCoord);
    vec4 color = vc * mix(tc1, tc2, 0.2f);
    FragColor = vec4(color.rgb * lightcolor, 1);
}
