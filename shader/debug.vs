#version 330 core

#include "lib/appdata.glsl"

#ifdef _VR_
uniform mat4 view;
uniform mat4 proj;
#endif

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
#ifdef _VR_
    vec3 pos = vec3(aPos.x * 0.4, aPos.y * 0.4, aPos.z * 0.4);
    gl_Position = proj * view *  vec4(pos, 1.0);
#else
    gl_Position = vec4(aPos, 0.9996, 1.0);
#endif
}
