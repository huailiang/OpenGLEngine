#version 330 core

#include "lib/appdata.glsl"

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
	gl_Position = vec4(aPos, -1.0, 1.0);
}
