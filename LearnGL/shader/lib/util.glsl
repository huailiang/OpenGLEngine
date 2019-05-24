#ifndef _STANDARD_
#define _STANDARD_

#include "lib/const.glsl"

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


// Unpack normal as DXT5nm (1, y, 1, x) or BC5 (x, y, 0, 1)
// Note neutral texture like "bump" is (0, 0, 1, 1) to work with both plain RGB normal and DXT5nm/BC5
vec3 UnpackNormalmapRGorAG(vec4 packednormal)
{
    packednormal.x *= packednormal.w;
    vec3 normal;
    normal.xy = packednormal.xy * 2 - 1;
    normal.z = sqrt(1 - saturate(dot(packednormal.xy, packednormal.xy)));
    return normal;
}


vec3 UnpackNormalmap(vec4 packednormal)
{
    vec3 normal;
#ifdef _DX5nm_
    normal = UnpackNormalmapRGorAG(packednormal);
#else
    normal.xy = packednormal.xy * 2 -1;
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
    vec4 kDecodeDot = vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0);
    return dot( enc, kDecodeDot );
}


float Luminance(vec3 rgb)
{
    return dot(rgb, ColorSpaceLuminance.rgb);
}


#endif
