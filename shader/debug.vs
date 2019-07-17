#version 330 core

#include "lib/appdata.glsl"

#ifdef _VR_
uniform mat4 model;
uniform mat4 proj;
#else
out vec2 TexCoords;
#endif

void main()
{
#ifdef _VR_
    gl_Position = proj * model * vec4(aPos, 0.0, 1.0);
#else
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.9, 1.0);
#endif
}
