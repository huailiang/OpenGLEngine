#version 330 core

#include "lib/appdata.glsl"
LayoutBaseVert2()

//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexcoord;
    gl_Position = vec4(aPos, -1.0, 1.0);
}
