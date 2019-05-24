#version 330 core


#ifndef _LIGHT_
#define _LIGHT_

//#define _SpotLight_


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


in vec3 worldPos;
in vec3 normal;
uniform vec3 viewPos;
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
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    return light.color * spec * material.specular;
}

vec3 Blinn_Specular()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    return light.color * spec * material.specular;
}


vec3 LightColor()
{
    vec3 ambient = vec3(0.8f) * material.ambient;
    vec3 diffuse = Diffuse();
    vec3 specular = Specular();

#ifdef  _SpotLight_
    lightDir = normalize(light.position - worldPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.25f, 1.0f);
#endif
    
#ifdef _PointLight_
    float distance =  length(light.position - worldPos);
    vec3 cst = light.constant;
    float atten = 1.0f/(cst.x+cst.y*distance+cst.z*distance*distance);
#endif

vec3 lightcolor = ambient + diffuse + specular;
    
#ifdef _PointLight_
    lightcolor = atten  * lightcolor;
#endif
    
#ifdef  _SpotLight_
    lightcolor *= intensity;
#endif

    return lightcolor;
}


#endif

#ifndef _STANDARD_
#define _STANDARD_

#ifndef _CONST_
#define _CONST_

#define e          2.7182818
#define HALF_MAX        65504.0 // (2 - 2^-10) * 2^15
#define HALF_MAX_MINUS1 65472.0 // (2 - 2^-9) * 2^15
#define EPSILON         1.0e-5
#define PI              3.14159265359
#define TWO_PI          6.28318530718
#define FOUR_PI         12.56637061436
#define INV_PI          0.31830988618
#define INV_TWO_PI      0.15915494309
#define INV_FOUR_PI     0.07957747155
#define HALF_PI         1.57079632679
#define INV_HALF_PI     0.636619772367


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


out vec4 FragColor;

in vec3 vertColor;
// in vec3 normal;
// in vec3 worldPos;
in vec2 texCoord;

uniform float scale;
uniform sampler2D texture1;
uniform sampler2D texture2;
// uniform vec3 viewPos;

void main()
{
    vec3 lightcolor = LightColor();
    
    vec4 vc = vec4(vertColor * 1, 1);
    vec4 tc1 = texture(texture1, texCoord);
    vec4 tc2 = texture(texture2, texCoord);
    vec4 color = vc * mix(tc1, tc2, 0.2f);
    FragColor = vec4(color.rgb * lightcolor, 1);
}
