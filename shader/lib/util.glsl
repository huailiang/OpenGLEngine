#ifndef _UTIL_
#define _UTIL_

#include "lib/const.glsl"
#include "lib/struct.glsl"

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
    return dot(enc, kDecodeDot);
}


/*
 * 灰度公式 区分线性空间和gammaf空间
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
    
    //if projCoords.xy 不在[0,1]区间，即shadowmap没有绘制，则不画shadow
    shadow = shadow * step(0.0, projCoords.x);
    shadow = shadow * step(projCoords.x, 1.0);
    shadow = shadow * step(0.0, projCoords.y);
    shadow = shadow * step(projCoords.y, 1.0);
    shadow = shadow * step(projCoords.z, 1.0);
    return shadow;
}


#endif
