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

    enum TEX_TYPE
    {
        TEXTURE,
        CUBEMAP,
    };
    
    class Texture
    {
    public:
        
        Texture(const char* path, EXT ext, GLuint* texID= nullptr, bool flipY= true, int wrap = GL_REPEAT);
        
        Texture(std::string path, GLuint* texID);
        
    public:
        GLuint textureID;
        int width, height;
        const char* path;
        bool mipmap;
        GLint wrap;
        TEX_TYPE type;
        EXT ext;
    };
    
}

#endif /* texture_h */
