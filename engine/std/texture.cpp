//
//  texture.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/29/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "texture.h"
#include <iostream>
#include "texmgr.h"
#ifdef _GLES_
#include "FilePath.h"
#else
#include "../ext/stb_image.h"
#endif


namespace engine
{

    Texture::Texture(const char* path, EXT ext, GLuint* texID, bool flipY, int wrap)
    {
#ifdef _GLES_
        ext = PVR;
#endif
        std::string spath(path);
        spath = getResPath(spath + getTextureExt(ext));
        this->path=spath.c_str();
        *texID = LoadTexture(flipY, ext, wrap);
    }
    
    Texture::Texture(std::string path, GLuint* texID)
    {
       *texID = LoadCubemap(path);
    }

    Texture::~Texture()
    {
        TexMgr::getInstance()->RemvTexture(textureID);
    }
    
    void Texture::SetTextureFormat(GLenum target, GLint filter, GLint wrap, GLint align)
    {
#ifdef _GLES_
        filter = GL_LINEAR; // es such as pvrtc is not support mipmap
#endif
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap);
        glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    }
    
    unsigned char* Texture::RealLoad(int *width, int *height, EXT ext, GLenum* glformat, int* level, int* bitsPerPixel)
    {
        unsigned char* data  = nullptr;
#ifndef _GLES_
        int fmt = 0;
        data = stbi_load(path, width, height, &fmt, 0);
        GLenum format = GetFormat(fmt);
        *glformat = format;
#else
        if (ext == PVR)
            data = LoadPvr(path, width, height, glformat, level, bitsPerPixel);
        else if(ext == TGA)
            data = LoadTGA(NULL, path, width, height, glformat);
#endif
        return data;
    }
    
    void Texture::Free(unsigned char* data, EXT ext)
    {
#ifndef _GLES_
        stbi_image_free(data);
#else
        if (ext == PVR)
            UnloadPvr();
        else
            free(data);
#endif
    }

    GLuint Texture::LoadTexture(bool flipY, EXT ext,int wrap, bool gen_mipmap)
    {
    #ifndef _GLES_
        stbi_set_flip_vertically_on_load(flipY);
    #endif
        
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        SetTextureFormat(GL_TEXTURE_2D, gen_mipmap ? GL_LINEAR_MIPMAP_LINEAR: GL_LINEAR, wrap);
        
        GLenum glformat = GL_RED;
        GLint level = 0;
        int bitsPerPixel =0;
        unsigned char* data = RealLoad(&width, &height,ext, &glformat, &level, &bitsPerPixel);
        if (data)
        {
            if(ext == PVR)
            {
                if(!gen_mipmap) level = 1;
                for (int mip = 0; mip < level; ++mip) {
                    GLsizei size = max(32, width * height * bitsPerPixel / 8);
                    glCompressedTexImage2D(GL_TEXTURE_2D, mip, glformat, width, height, 0, size, data);
                    data += size;
                    width >>= 1; height >>= 1;
                }
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, glformat, GL_UNSIGNED_BYTE, data);
                if(gen_mipmap) glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        else
        {
            std::cout << "Texture failed: " << path << std::endl;
        }
        Free(data,ext);
        return textureID;
    }
    
    GLuint Texture::LoadCubemap(std::string cubepath)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
#ifndef _GLES_
        stbi_set_flip_vertically_on_load(false);
#endif
        string str[]={"right","left","top","bottom","front","back"};
        GLenum glformat = GL_RED;
        GLint level = 0;
        int bitsPerPixel =0;
        for (uint i = 0; i < 6; i++)
        {
            EXT ext = JPG;
            std::string post = EX_JPG;
#ifdef _GLES_
            ext = PVR;
            post = EX_PVR;
#endif

            std::string face = getResPath(cubepath+str[i]+post);
            path = face.c_str();
            unsigned char *data = RealLoad(&width, &height, ext, &glformat, &level, &bitsPerPixel);
            if(ext==PVR)
            {
                GLsizei size = max(32, width * height * bitsPerPixel / 8);
                glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glformat, width, height, 0, size, data);
            }
            else
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glformat, width, height, 0, glformat, GL_UNSIGNED_BYTE, data);
            
            Free(data,ext);
        }
        SetTextureFormat(GL_TEXTURE_CUBE_MAP, GL_LINEAR, GL_CLAMP_TO_EDGE);
        
        return textureID;
    }
    

    ivec2 Texture::GetShape()
    {
        return ivec2(width,height);
    }

    GLenum Texture::GetFormat(int format)
    {
        GLenum glformat;
        if (format == 1)
            glformat = GL_RED;
        else if (format == 3)
            glformat = GL_RGB;
        else if (format == 4)
            glformat = GL_RGBA;
        else
        {
            glformat = GL_RED;
            std::cout<<"texture is not support the format"<<format<<std::endl;
        }
        return glformat;
    }

}
