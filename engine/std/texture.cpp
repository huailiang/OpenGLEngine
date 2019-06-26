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
        std::string spath(path);
        spath = getResPath(spath + getTextureExt(ext));
        this->path=spath.c_str();
        *texID = TexMgr::getInstance()->LoadTexture(this->path, flipY, ext, &width, &height, wrap);
    }
    
    Texture::Texture(std::string path, GLuint* texID)
    {
       *texID = TexMgr::getInstance()->LoadCubemap(path);
    }

    
    ivec2 Texture::GetShape()
    {
        return ivec2(width,height);
    }

}
