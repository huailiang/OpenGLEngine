#version 330 core


#define _Light_

out vec4 FragColor;

in vec2 TexCoords;

#ifdef _Light_

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


#endif // _Light_


uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
#ifdef _Light_
    vec3 litColor = LightColor();
    FragColor = FragColor * vec4(litColor,1);
#endif
}
