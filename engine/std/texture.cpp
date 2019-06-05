//
//  texture.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/29/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "texture.h"
#include <iostream>
#ifdef _GLES_
#include "FilePath.h"
#else
#include "../ext/stb_image.h"
#endif

namespace engine
{

    TTexture::TTexture(const char* path, unsigned int* texID, bool flipY, int wrap)
    {
        std::string spath(path);
        spath = WORKDIR + spath;
        this->path=spath.c_str();
        *texID = LoadTexture(flipY, wrap);
    }


    TTexture::~TTexture()
    {
    //    glad_glDeleteTextures(1, &textureID);
    }

    unsigned int TTexture::LoadTexture(bool flipY, int wrap)
    {
    #ifndef _GLES_
        stbi_set_flip_vertically_on_load(flipY);
    #endif
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    #ifndef _GLES_
        unsigned char *data = stbi_load(path, &width, &height, &format, 0);
    #else
        char *data = LoadImage(std::string(path), &width, &height);
    #endif
        if (data)
        {
    #ifndef _GLES_
            GLenum glformat = GetFormat();
    #else
            GLenum glformat = GL_RGBA;
    #endif
            glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, glformat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
        }
    #ifndef _GLES_
        stbi_image_free(data);
    #else
        free(data);
    #endif
        return textureID;
    }

    ivec2 TTexture::GetShape()
    {
        return ivec2(width,height);
    }

    GLenum TTexture::GetFormat()
    {
        GLenum glformat;
        if (this->format == 1)
            glformat = GL_RED;
        else if (this->format == 3)
            glformat = GL_RGB;
        else if (this->format == 4)
            glformat = GL_RGBA;
        else
        {
            glformat = GL_RED;
            std::cout<<"texture is not support the format"<<this->format<<std::endl;
        }
        return glformat;
    }

}
