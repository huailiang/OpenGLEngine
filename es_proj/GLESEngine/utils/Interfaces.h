//
//  Interfaces.h
//  GLESEngine
//
//  Created by 彭怀亮 on 6/19/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef Interfaces_h
#define Interfaces_h

#include <vector>
#include <string>


using std::string;


/*
 * 关于贴图压缩和工具的使用参考：
 *      https://huailiang.github.io/blog/2019/texture/
 */

enum TextureFormat {
    TextureFormatGray,
    TextureFormatGrayAlpha,
    TextureFormatRgb,
    TextureFormatRgba,
    TextureFormatPvrtcRgb2,
    TextureFormatPvrtcRgba2,
    TextureFormatPvrtcRgb4,
    TextureFormatPvrtcRgba4,
    TextureFormat565,
    TextureFormat5551,
};


struct TextureDescription {
    TextureFormat Format;
    int BitsPerComponent;
    int Height;
    int Width;
    int MipCount;
};

struct IResourceManager {
    virtual string GetResourcePath() const = 0;
    virtual TextureDescription LoadPngImage(const string& filename) = 0;
    virtual TextureDescription LoadPvrImage(const string& filename) = 0;
    virtual TextureDescription LoadImage(const string& filename) = 0;
    virtual void* GetImageData() = 0;
    virtual void UnloadImage() = 0;
    virtual ~IResourceManager() {}
};


#ifdef __APPLE__
IResourceManager* CreateiOSAsset();
#endif

#endif /* Interfaces_h */
