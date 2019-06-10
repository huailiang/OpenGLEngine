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


#endif // _STRUCT_
