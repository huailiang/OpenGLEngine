//
//  texture.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/10/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include <glm/glm.hpp>
#include "asset.h"


using namespace glm;

namespace engine
{

    class Texture
    {
    public:
        
        GLuint textureID;
        
        Texture(const char* path, EXT ext, GLuint* texID= nullptr, bool flipY= true, int wrap = GL_REPEAT);
        
        ~Texture();
        
        ivec2 GetShape();
        
        GLenum GetFormat();
        
        GLuint LoadTexture(bool flipY, EXT ext, int wrap);
        
        
    private:
        int width, height, format;
        
        const char* path;
        
    };


}

#endif /* texture_h */
