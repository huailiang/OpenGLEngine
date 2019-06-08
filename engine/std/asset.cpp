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
    
    std::map<unsigned int,int> asset_map;
    
    bool initial()
    {
        return true;
    }
    
    bool isTexure(int ext)
    {
        return ext && 0x00f0 > 0;
    }
    
    
    bool isEngineAsset(int ext)
    {
        return ext && 0x0f00 > 0;
    }
    
    
    std::string getResPath(std::string path)
    {
#ifdef _QT_EDIT_
        return WORKDIR + std::to_string(Hash(path));
#else
#ifdef _GLES_
        size_t idx = path.rfind('/');
        path = path.substr(idx+1);
        return getPath(std::to_string(Hash(path)));
#else
        return "bundle/"+ std::to_string(Hash(path));
#endif // _GLES_
#endif // _QT_EDIT_
    }
    
    
}


