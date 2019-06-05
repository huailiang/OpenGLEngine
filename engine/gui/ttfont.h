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

namespace engine
{

    
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

        float RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 color);
        
        int initial();

    private:
        
        ~TTFont();
        
    private:
        unsigned int VBO,VAO;
        Shader* shader;
        map<GLchar, Character> Characters;
    };


}

#endif /* ttfont_h */
