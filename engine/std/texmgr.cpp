//
//  texturemgr.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "texmgr.h"
#ifdef _GLES_
#include "FilePath.h"
#else
#include "../ext/stb_image.h"
#endif

namespace engine
{
    
    TexMgr TexMgr::instance;
    
    
    bool TexMgr::FindTexture(GLuint& texid, size_t& idx)
    {
        for (size_t i=0; i<_num_tex; i++)
        {
            if (_texids[i] == texid)
            {
                idx = i;
                return true;
            }
        }
        return false;
    }

    
    bool TexMgr::FindTexture(string& texture, GLuint& texid, size_t& idx)
    {
        for (size_t i=0; i<_num_tex; i++)
        {
            if (_textures[i] == texture)
            {
                texid = _texids[i];
                idx = i;
                return true;
            }
        }
        return false;
    }

    
    void TexMgr::LoadTex(std::string& texture, EXT ext, GLuint& texid)
    {
        size_t idx = 0;
        int width, height;
        if(texture.empty() || ext== NONE) return;
        if (!FindTexture(texture, texid, idx))
        {
            if (CUBE == ext)
                texid = LoadCubemap(texture.c_str());
            else
            {
                std::string spath = getResPath(texture + getTextureExt(ext));
                texid = LoadTexture(spath.c_str(), true, ext, &width, &height, GL_REPEAT);
            }
            _textures.push_back(texture);
            _texids.push_back(texid);
            _references.push_back(1);
            _num_tex++;
        }
        else
        {
            _references[idx]++;
        }
    }
    
    
    void TexMgr::RealRemove(size_t idx)
    {
        size_t ref=_references[idx];
        if (ref < 1)
        {
            std::cerr<<"texmgr inner error"<<std::endl;
        }
        else if(ref == 1)
        {
            glDeleteTextures(1, &_texids[idx]);
            _texids.erase(_texids.begin() + idx);
            _textures.erase(_textures.begin() + idx);
            _references.erase(_references.begin() + idx);
            _num_tex--;
        }
        else
        {
            _references[idx]--;
        }
    }

    bool TexMgr::RemvTexture(GLuint texid)
    {
        size_t idx = 0;
        if (FindTexture(texid, idx))
        {
            RealRemove(idx);
            return  true;
        }
        return false;
    }
    
    bool TexMgr::RemvTexture(std::string& texture)
    {
        size_t idx = 0;
        GLuint texid;
        if (FindTexture(texture, texid, idx))
        {
            RealRemove(idx);
            return true;
        }
        return  false;
    }
    
    
    void TexMgr::UnloadAllTexture()
    {
        for (size_t i=0; i<_num_tex; i++)
        {
            GLuint texid = _texids[i];
            glDeleteTextures(1, &texid);
        }
        _textures.clear();
        _texids.clear();
        _references.clear();
        _num_tex=0;
    }
    
    void TexMgr::SetTextureFormat(GLenum target, GLint filter, GLint wrap, GLint align)
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
    
    
    GLuint TexMgr::LoadTexture(const char* path, bool flipY, EXT ext, int* width, int* height, int wrap, bool gen_mipmap)
    {
#ifndef _GLES_
        stbi_set_flip_vertically_on_load(flipY);
#endif
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        SetTextureFormat(GL_TEXTURE_2D, gen_mipmap ? GL_LINEAR_MIPMAP_LINEAR: GL_LINEAR, wrap);
        
        GLenum glformat = GL_RED;
        GLint level = 0;
        int bitsPerPixel =0;
        unsigned char* data = RealLoad(path, width, height,ext, &glformat, &level, &bitsPerPixel);
        if (data)
        {
            if(ext == PVR)
            {
                if(!gen_mipmap) level = 1;
                for (int mip = 0; mip < level; ++mip) {
                    GLsizei size = max(32, *width * *height * bitsPerPixel / 8);
                    glCompressedTexImage2D(GL_TEXTURE_2D, mip, glformat, *width, *height, 0, size, data);
                    data += size;
                    *width >>= 1; *height >>= 1;
                }
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, glformat, *width, *height, 0, glformat, GL_UNSIGNED_BYTE, data);
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
    
    GLuint TexMgr::LoadCubemap(std::string cubepath)
    {
        unsigned int textureID;
        int width,height;
        const char* path;
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
            unsigned char *data = RealLoad(path, &width, &height, ext, &glformat, &level, &bitsPerPixel);
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
    
    
    void TexMgr::Free(unsigned char* data,EXT ext)
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
    
    
    unsigned char* TexMgr::RealLoad(const char* path, int *width, int *height,EXT ext, GLenum* glformat, int* level, int* bitsPerPixel)
    {
        unsigned char* data  = nullptr;
        
#ifndef _GLES_
        int fmt = 0;
        if(ext == HDR)
            data = (unsigned char*)stbi_loadf(path, width, height, &fmt,0);
        else
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
    
    GLenum TexMgr::GetFormat(int format)
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

