#version 330 core

#define _DEBUG_DEPTH_
//#define _DEBUG_COLOR_

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

#ifdef _DEBUG_DEPTH_

uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

#endif

void main()
{    
    vec4 color = texture(texture1, TexCoords);
#ifdef _DEBUG_DEPTH_
    float depth = LinearizeDepth(color.r) / far_plane ;
    FragColor = vec4(depth, depth, depth, 1);
#else
    FragColor = color;
#endif
}
