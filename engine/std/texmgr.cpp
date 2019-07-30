//
//  texturemgr.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "texmgr.hpp"
#ifdef _GLES_
#include "FilePath.h"
#else
#include "../ext/stb_image.h"
#endif

namespace engine
{
    
    TexMgr TexMgr::instance;

    
    Texture* TexMgr::FindTexture(GLuint& texid)
    {
        loop (_num_tex)
        {
            if (_texids[i].textureID == texid)
            {
                return &_texids[i];
            }
        }
        return nullptr;
    }
    
    bool TexMgr::FindTexture(GLuint& texid, size_t& idx)
    {
        loop (_num_tex)
        {
            if (_texids[i].textureID == texid)
            {
                idx = i;
                return true;
            }
        }
        return false;
    }

    
    bool TexMgr::FindTexture(string& texture, GLuint& texid, size_t& idx)
    {
        loop (_num_tex)
        {
            if (_texids[i].path == texture)
            {
                texid = _texids[i].textureID;
                idx = i;
                return true;
            }
        }
        return false;
    }

    
    void TexMgr::LoadTex(const char* path, bool flipY, Texture* tex)
    {
        size_t idx = 0;
        std::string texture= std::string(path);
        if (!FindTexture(texture, tex->textureID, idx))
        {
            if (CUBEMAP == tex->type)
                tex->textureID = LoadCubemap(path,tex);
            else
                tex->textureID = LoadTexture(path, flipY, tex);
            _texids.push_back(*tex);
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
            _texids.erase(_texids.begin() + idx);
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
            GLuint texid = _texids[i].textureID;
            glDeleteTextures(1, &texid);
        }
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
        if(filter == GL_LINEAR_MIPMAP_LINEAR) filter = GL_LINEAR;
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap);
        glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    }
    
    
    GLuint TexMgr::LoadTexture(const char* path, bool flipY, Texture* tex)
    {
#ifndef _GLES_
        stbi_set_flip_vertically_on_load(flipY);
#endif
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        SetTextureFormat(GL_TEXTURE_2D, tex->mipmap ? GL_LINEAR_MIPMAP_LINEAR: GL_LINEAR, tex->wrap);
        GLenum glformat = GL_RED;
        GLint level = 0;
        int bitsPerPixel =0;
        unsigned char* data = (unsigned char*)RealLoad(path, &tex->width, &tex->height, tex->ext, &glformat, &level, &bitsPerPixel);
        tex->data = data;
        if (data)
        {
            int width = tex->width, height = tex->height;
            if(tex->ext == _PVR)
            {
                if(!tex->mipmap) level = 1;
                for (int mip = 0; mip < level; ++mip) {
                    GLsizei size = max(32, width * height * bitsPerPixel / 8);
                    glCompressedTexImage2D(GL_TEXTURE_2D, mip, glformat, width, height, 0, size, data);
                    data += size;
                    width >>= 1; height >>= 1;
                }
            }
            else
            {
                if(tex->ext == _HDR)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, glformat, GL_FLOAT, data);
                else
                    glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, glformat, GL_UNSIGNED_BYTE, data);
                if(tex->mipmap) glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        else
        {
            std::cout << "Texture failed: " << path << std::endl;
        }
        Free(data,tex->ext);
        return textureID;
    }
    
    GLuint TexMgr::LoadCubemap(std::string cubepath,Texture* tex)
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
            EXT ext = _JPG;
            std::string post = EX_JPG;
#ifdef _GLES_
            ext = _PVR;
            post = EX_PVR;
#endif
            
            std::string face = getResPath(cubepath+str[i]+post);
            path = face.c_str();
            void *data = RealLoad(path, &width, &height, ext, &glformat, &level, &bitsPerPixel);
            tex->data = data;
            if(ext==_PVR)
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
    
    
    void TexMgr::Free(void* data,EXT ext)
    {
#ifndef _GLES_
        stbi_image_free(data);
#else
        if (ext == _PVR)
            UnloadPvr();
        else
            free(data);
#endif
    }
    
    
    void* TexMgr::RealLoad(const char* path, int *width, int *height,EXT ext, GLenum* glformat, int* level, int* bitsPerPixel)
    {
        void* data  = nullptr;
        
#ifndef _GLES_
        int fmt = 0;
        if(ext == _HDR)
            data = stbi_loadf(path, width, height, &fmt,0);
        else
            data = stbi_load(path, width, height, &fmt, 0);
        GLenum format = GetFormat(fmt);
        *glformat = format;
#else
        if (ext == _PVR)
            data = LoadPvr(path, width, height, glformat, level, bitsPerPixel);
        else if(ext == _TGA)
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

