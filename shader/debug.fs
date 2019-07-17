#version 330 core

out vec4 FragColor;

#ifndef _VR_
in vec2 TexCoords;
uniform sampler2D texture1;
#endif

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
#ifdef _VR_
    FragColor = vec4(1,0,0,0.4);
#else
#ifdef _FLIP_Y_
    vec4 color = texture(texture1, vec2(TexCoords.x, 1.0 - TexCoords.y));
#else
    vec4 color = texture(texture1, TexCoords);
#endif // _FLIP_Y_
#ifdef _DEBUG_DEPTH_
    float depth = LinearizeDepth(color.r) / far_plane ;
    FragColor = vec4(depth, depth, depth, 1);
#else
    FragColor = color;
#endif //  _DEBUG_DEPTH_
    
#endif // _VR_
    
}
