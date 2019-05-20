#version 330 core

#ifdef  _SpotLight_
#define _PointLight_
#endif

out vec4 FragColor;

in vec2 TexCoords;
in vec4 LightPos;

// sample texture
uniform sampler2D texture1;
//apply shadow
uniform sampler2D shadow;


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

uniform Light light;


float ShadowCalculation()
{
    // perform perspective divide
    vec3 projCoords = LightPos.xyz / LightPos.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    if(projCoords.z > 1.0) shadow = 0.0;
    return shadow;
}


void main()
{
    vec4 color = texture(texture1, TexCoords);
    float shadow = ShadowCalculation();
    color = (1.0 - shadow) * color;
    FragColor = color;
}
