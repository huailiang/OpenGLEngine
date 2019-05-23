#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 arg;
} gs_in[];

out vec3 fColor;

void Generate(vec4 position, float half_w, float half_h)
{
    fColor = vec3(0.5f);
    gl_Position = position + vec4(-half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, -half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-half_w, half_h, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(half_w, half_h, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec4 arg = gs_in[0].arg;
    arg.x = arg.x * 2.0f - 1.0f;
    arg.y = arg.y * 2.0f - 1.0f;
    Generate(vec4(arg.x, arg.y, 0.9, 1.0), arg.z*0.5f, arg.w*0.5f);
}
