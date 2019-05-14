#version 330 core
//The shader can be used to post-processing

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void gray()
{
    vec4 col = texture(screenTexture, TexCoords);
    float average = (col.r + col.g + col.b) * 0.33f;
    col.r = average;
    col.g = average;
    col.b = average;
    col.a = 1.0f;
    FragColor = col;
}

void blur()
{
    const float offset = 1.0 / 60.0;
    
    vec2 offsets[9] = vec2[](
                             vec2(-offset,  offset), // 左上
                             vec2( 0.0f,    offset), // 正上
                             vec2( offset,  offset), // 右上
                             vec2(-offset,  0.0f),   // 左
                             vec2( 0.0f,    0.0f),   // 中
                             vec2( offset,  0.0f),   // 右
                             vec2(-offset, -offset), // 左下
                             vec2( 0.0f,   -offset), // 正下
                             vec2( offset, -offset)  // 右下
                             );
    
    float kernel[9] = float[](
                              1.0 / 16, 2.0 / 16, 1.0 / 16,
                              2.0 / 16, 4.0 / 16, 2.0 / 16,
                              1.0 / 16, 2.0 / 16, 1.0 / 16
                              );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    FragColor = vec4(col, 1.0);
}


void main()
{
//    blur();
//    gray();
    
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}
