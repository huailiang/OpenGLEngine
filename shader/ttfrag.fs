#version 330 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    color = vec4(textColor, texture(text, texCoord).r);
//    color = vec4(texture(text, texCoord).w,0,0,1);
}
