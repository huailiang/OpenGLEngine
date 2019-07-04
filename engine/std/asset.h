//
//  asset.hpp
//  GLESEngine
//
//  Created by 彭怀亮 on 6/8/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//


/*
 资源管理器 请配合工具使用
*/

#ifndef asset_h
#define asset_h


#include "../engine.h"

namespace engine
{
    
    #define _PNG   0x0010  //.png
    #define _JPG   0x0020  //.jpg
    #define _TGA   0x0030  //.tga
    #define _BMP   0x0040  //.bmp
    #define _HDR   0x0050  //.hdr
    #define _PVR   0x0060  //.pvr

    #define _FBX   0x0001 //.fbx
    #define _MTL   0x0002 //.mtl
    #define _OBJ   0x0003 //.obj
    #define _ANIM  0x0004 //.anim
    #define _TTF   0x0005 //.ttf
    
    #define _SUM   0x0100 //.sum
    #define _MESH  0x0200 //.mesh
    #define _MAT  0x0300 //.mat
    
    
    #define EX_PNG   ".png"
    #define EX_JPG   ".jpg"
    #define EX_TGA   ".tga"
    #define EX_BMP   ".bmp"
    #define EX_HDR   ".hdr"
    #define EX_PVR   ".pvr"
    
    #define EX_FBX   ".fbx"
    #define EX_MTL   ".mtl"
    #define EX_OBJ   ".obj"
    #define EX_ANIM  ".anim"
    #define EX_TTF   ".ttf"
    
    #define EX_SUM   ".sum"
    #define EX_MESH  ".mesh"
    #define EX_MAT   ".mat"
    
    
    typedef unsigned int EXT;

    bool isTexure(const EXT ext);
    
    std::string getTextureExt(const EXT ext);
    
    EXT getTextureFormat(const char* ext);
    
    bool isEngineAsset(const EXT ext);
     
    std::string getResPath(const std::string path);
    
}




#endif /* asset_hpp */
