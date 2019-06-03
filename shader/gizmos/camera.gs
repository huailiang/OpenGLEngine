#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 8) out;

in VS_OUT {
    vec4 ltn;
    vec4 lbn;
    vec4 rtn;
    vec4 rbn;
    vec4 ltf;
    vec4 lbf;
    vec4 rtf;
    vec4 rbf;
} gs_in[];

out vec3 fColor;

void Generate()
{
    fColor = vec3(0.5f);
    gl_Position = gs_in[0].ltn;
    EmitVertex();
    gl_Position = gs_in[0].lbn;
    EmitVertex();
    gl_Position = gs_in[0].rtn;
    EmitVertex();
    gl_Position = gs_in[0].rbn;
    EmitVertex();
    gl_Position = gs_in[0].ltf;
    EmitVertex();
    gl_Position = gs_in[0].lbf;
    EmitVertex();
    gl_Position = gs_in[0].rtf;
    EmitVertex();
    gl_Position = gs_in[0].rbf;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    Generate();
}
