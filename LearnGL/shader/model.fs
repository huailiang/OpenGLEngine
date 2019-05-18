#version 330 core

#define _Light_
//#define  _SpotLight_
out vec4 FragColor;

in vec2 TexCoords;
#ifdef _Light_
in vec3 normal;
in vec3 worldPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


struct Light
{
    vec3 direction;
    vec3 color;
#ifdef  _SpotLight_
#define _PointLight_
    float cutOff;
    float outerCutOff;
#endif // _SpotLight_
    
#ifdef _PointLight_
    vec3 position;
    vec3 constant;
#endif // _PointLight_
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec3 calLight()
{
    // ambient
    vec3 ambient = vec3(0.8f) * material.ambient;

    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * material.diffuse;

    //speular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.color * spec * material.specular;

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

#endif // _Light_


uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
#ifdef _Light_
    vec3 litColor = calLight();
    FragColor = FragColor * vec4(litColor,1);
#endif
}
