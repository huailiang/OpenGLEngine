#version 330 core

#include "lib/phong.glsl"
#include "lib/util.glsl"

out vec4 FragColor;

in vec2 texCoords;
in vec4 fragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

void main()
{
    vec3 color = texture(diffuseTexture, texCoords).rgb;
    vec3 ambient = 0.3 * color;
    vec3 diffuse = Diffuse();
    vec3 specular = Specular();
    
    // calculate shadow
    float shadow = ShadowCalculation(shadowMap, fragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    FragColor = vec4(lighting, 1.0);
}
