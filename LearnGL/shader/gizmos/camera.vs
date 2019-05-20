#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec4 constant; // <left, top, near, far>

out VS_OUT
{
    vec4 constant;
}vs_out;



void main()
{
    vs_out.constant = constant;
    gl_Position = vec4(0, 0, -1.0, 1.0);
}
