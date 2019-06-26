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


namespace engine
{

    Texture::Texture(const char* path, EXT ext, GLuint* texID, bool flipY, int wrap,bool mipmap)
    {
#ifdef _GLES_
        ext = PVR;
#endif
        this->type = TEXTURE;
        this->path = path;
        this->ext = ext;
        this->wrap = wrap;
        this->mipmap = mipmap;
        std::string spath(path);
        spath = getResPath(spath + getTextureExt(ext));
        TexMgr::getInstance()->LoadTex(spath.c_str(), flipY, this);
        *texID = this->textureID;
    }
    
    
    Texture::Texture(const char* path, GLuint* texID)
    {
        this->type = CUBEMAP;
        this->path  = path;
        TexMgr::getInstance()->LoadTex(path,true,this);
        *texID = this->textureID;
    }

}
