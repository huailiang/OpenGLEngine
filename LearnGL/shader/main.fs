#version 330 core

#define _PointLight_

out vec4 FragColor;

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
#endif

#ifdef _PointLight_
    vec3 position;
    vec3 constant;
#endif
};



in vec3 vertColor;
in vec3 normal;
in vec3 worldPos;
in vec2 texCoord;
uniform float scale;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = vec3(0.1f, 0.1f, 0.1f) * material.ambient;
    
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * material.diffuse;
    
    //speular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color *spec * material.specular;

#ifdef _PointLight_
    float distance =  length(light.position - worldPos);
    vec3 cst = light.constant;
    float atten = 1.0f/(cst.x+cst.y*distance+cst.z*distance*distance);
#endif
    
    vec3 lightcolor = ambient + diffuse + specular;
    
#ifdef _PointLight_
    lightcolor = atten * lightcolor;
#endif
    
    vec4 vc = vec4(vertColor * 1, 1);
    vec4 tc1 = texture(texture1, texCoord);
    vec4 tc2 = texture(texture2, texCoord);
    vec4 color = vc * mix(tc1, tc2, 0.2f);
    FragColor = vec4(lightcolor, 1);// vec4(color.rgb * lightcolor, 1);
}
