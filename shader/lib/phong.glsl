#ifndef _LIGHT_
#define _LIGHT_

#include "lib/camera.glsl"
#include "lib/struct.glsl"


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
    vec3 ambient = vec3(0.1f) * material.ambient;
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
    diffuse *= 1.6;
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
