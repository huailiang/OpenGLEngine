//
//  texturemgr.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "texmgr.h"


namespace engine
{
    
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

    
    void TexMgr::LoadTex(std::string& texture, GLuint& texid)
    {
        size_t idx = 0;
        if (!FindTexture(texture, texid, idx))
        {
            TTexture(texture.c_str(), &texid);
            
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

    
}

