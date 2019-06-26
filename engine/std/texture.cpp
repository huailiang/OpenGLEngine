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

    Texture::Texture(const char* path, EXT ext, GLuint* texID, bool flipY, int wrap)
    {
#ifdef _GLES_
        ext = PVR;
#endif
        this->type = TEXTURE;
        this->path = path;
        this->ext = ext;
        this->wrap = wrap;
        this->mipmap = true;
        std::string spath(path);
        spath = getResPath(spath + getTextureExt(ext));
        TexMgr::getInstance()->LoadTex(spath.c_str(), flipY, this);
        *texID = this->textureID;
    }
    
    Texture::Texture(std::string path, GLuint* texID)
    {
        this->type = CUBEMAP;
        this->path  = path.c_str();
        TexMgr::getInstance()->LoadTex(this->path,true,this);
        *texID = this->textureID;
    }

}
