#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 texCoord;

uniform mat4 projection;

void main()
{
    
    /*
     * projection 会翻转z， 所以这里设的z=0.99
     */
    gl_Position = projection * vec4(vertex.xy, 0.99f, 1.0f);
    
    texCoord = vertex.zw;
}
