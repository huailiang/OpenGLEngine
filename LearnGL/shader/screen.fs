#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{    
    vec4 color = texture(screenTexture, TexCoords);
    color.a = 1.0f;
    FragColor = color;
}
