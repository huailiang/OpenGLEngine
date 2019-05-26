//
//  ttfont.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/9/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef ttfont_h
#define ttfont_h

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../common.h"
#include "../std/shader.h"

using namespace std;
using namespace glm;

struct Character
{
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

#define FONT_SIZE 20

class TTFont
{
    DeclareSington(TTFont)
    
public:

    float RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 color)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader->use();
        shader->setVec3("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);
        
        GLfloat start = x;
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];
            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };
            
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(DRAW_MODE, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        return x - start;
    }
    
    int initial()
    {
        shader =new Shader("ttfont.vs","ttfrag.fs");
        mat4 proj = ortho(0.0f, 800.0f, 0.0f, 600.0f);
        shader->use();
        shader->setMat4("projection", proj);
        
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        
        FT_Face face;
        if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        
        FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        // Load first 128 characters of ASCII set
        for (GLubyte c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                         GL_TEXTURE_2D,
                         0,
                         GL_RED,
                         face->glyph->bitmap.width,
                         face->glyph->bitmap.rows,
                         0,
                         GL_RED,
                         GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer
                         );
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            Character character = { texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint)face->glyph->advance.x
            };
            Characters.insert(std::pair<GLchar, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        return 0;
    }

private:
    
    ~TTFont()
    {
        delete shader;
        Characters.clear();
    }
    
    unsigned int VBO,VAO;
    Shader* shader;
    map<GLchar, Character> Characters;
};



#endif /* ttfont_h */
