#version 330 core

#define _Instance_

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in mat4 matrix;

out vec2 TexCoords;

uniform mat4 mvp;

void main()
{
    TexCoords = aTexCoords;
#ifdef _Instance_
    gl_Position = mvp * matrix  * vec4(aPos, 1.0);
#else
    gl_Position = mvp  * vec4(aPos, 1.0);
#endif
}
