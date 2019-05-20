#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec4 arg; // <left, top, near, far>
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


out VS_OUT {
    vec4 ltn;
    vec4 lbn;
    vec4 rtn;
    vec4 rbn;
    vec4 ltf;
    vec4 lbf;
    vec4 rtf;
    vec4 rbf;
} vs_out;


void main()
{
    vec4 vPos = view * model * vec4(aPos,1);
    vs_out.ltn = projection * (vPos + vec4(-arg.x,  arg.y, -arg.z, 0));
    vs_out.lbn = projection * (vPos + vec4(-arg.x, -arg.y, -arg.z, 0));
    vs_out.rtn = projection * (vPos + vec4(arg.x,  arg.y,  -arg.z, 0));
    vs_out.rbn = projection * (vPos + vec4(arg.x, -arg.y,  -arg.z, 0));
    vs_out.ltf = projection * (vPos + vec4(-arg.x, arg.y,  -arg.w, 0));
    vs_out.lbf = projection * (vPos + vec4(-arg.x, -arg.y, -arg.w, 0));
    vs_out.rtf = projection * (vPos + vec4(arg.x,  arg.y,  -arg.w, 0));
    vs_out.rbf = projection * (vPos + vec4(arg.x, -arg.y,  -arg.w, 0));
    gl_Position = vec4(aPos, 1.0);
}
