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
#include "asset.hpp"


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
        
        Texture(const char* path, EXT ext, GLuint* texID= nullptr, bool flipY= true, int wrap = GL_REPEAT, bool mipmap = false);
        
        Texture(const char* path, GLuint* texID);
        
    public:
        
        // genTexture 生成的id
        GLuint textureID = 0;
        
        //从文件头里读的长和宽
        int width, height;
        
        // 这里记录的是转换前的路径
        const char* path;
        
        // 是否使用了mipmap
        bool mipmap;
        
        // wrap mode
        GLint wrap;
        
        // texture or cubemap
        TEX_TYPE type;
        
        EXT ext;
        
        void* data = nullptr;
    };
    
}

#endif /* texture_h */
