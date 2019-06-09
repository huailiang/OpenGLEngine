//
//  asset.cpp
//  GLESEngine
//
//  Created by 彭怀亮 on 6/8/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "asset.h"

#ifdef _GLES_
#include "FilePath.h"
#endif


namespace engine
{
    
    bool isTexure(EXT ext)
    {
        return (ext & 0x00f0) > 0;
    }
    
    
    std::string getTextureExt(EXT ext)
    {
        if(!isTexure(ext))
        {
            std::cerr<<ext<<" is not a texure format"<<std::endl;
            return "";
        }
        std::string exts[] = {".png",".jpg",".tga",".bmp", ".hdr"};
        return exts[(ext>>4)-1];
    }
    
    
    EXT getTextureFormat(const char* ext)
    {
        std::string exts[] = {".png",".jpg",".tga",".bmp", ".hdr"};
        for (int i=0; i<5; i++) {
            if(strcmp(exts[i].c_str(),ext)==0)
            {
                return (i+1)<<4;
            }
        }
        return NONE;
    }
    
    
    bool isEngineAsset(EXT ext)
    {
        return (ext & 0x0f00) > 0;
    }
    
    
    std::string getResPath(std::string path)
    {
#ifdef _QT_EDIT_
        std::string xpath = std::string(WORKDIR) + "bundle/";
        return xpath + std::to_string(Hash(path));
#else
#ifdef _GLES_
        return getPath(std::to_string(Hash(path)));
#else
        return "bundle/"+ std::to_string(Hash(path));
#endif // _GLES_
#endif // _QT_EDIT_
    }
    
    
}


