//
//  ResourcesManager.m
//  GLESEngine
//
//  Created by 彭怀亮 on 6/19/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import "Interfaces.h"
#import "PowerVR/PVRTTexture.h"

using namespace std;


class ResourcesManager :public IResourceManager
{
    
public:
    
    ResourcesManager()
    {
        m_imageData = 0;
    }
    
    string GetResourcePath() const
    {
        NSString* bundlePath =[[NSBundle mainBundle] resourcePath];
        return [bundlePath UTF8String];
    }
    
    TextureDescription LoadImage(const string& file)
    {
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
        
        TextureDescription description;
        description.Width = (int)CGImageGetWidth(uiImage.CGImage);
        description.Height = (int)CGImageGetHeight(uiImage.CGImage);
        description.BitsPerComponent = 8;
        description.Format = TextureFormatRgba;
        description.MipCount = 1;
        m_hasPvrHeader = false;
        
        int bpp = description.BitsPerComponent / 2;
        int byteCount = description.Width * description.Height * bpp;
        unsigned char* data = (unsigned char*) calloc(byteCount, 1);
        
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
        CGContextRef context = CGBitmapContextCreate(data,
                                                     description.Width,
                                                     description.Height,
                                                     description.BitsPerComponent,
                                                     bpp * description.Width,
                                                     colorSpace,
                                                     bitmapInfo);
        CGColorSpaceRelease(colorSpace);
        CGRect rect = CGRectMake(0, 0, description.Width, description.Height);
        CGContextDrawImage(context, rect, uiImage.CGImage);
        CGContextRelease(context);
        
        m_imageData = [NSData dataWithBytesNoCopy:data length:byteCount freeWhenDone:YES];
        return description;
    }
    
    TextureDescription GenerateCircle()
    {
        TextureDescription description;
        description.Width = 256;
        description.Height = 256;
        description.BitsPerComponent = 8;
        description.Format = TextureFormatRgba;
        
        int bpp = description.BitsPerComponent / 2;
        int byteCount = description.Width * description.Height * bpp;
        unsigned char* data = (unsigned char*) calloc(byteCount, 1);
        
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
        CGContextRef context = CGBitmapContextCreate(data,
                                                     description.Width,
                                                     description.Height,
                                                     description.BitsPerComponent,
                                                     bpp * description.Width,
                                                     colorSpace,
                                                     bitmapInfo);
        CGColorSpaceRelease(colorSpace);
        
        CGRect rect = CGRectMake(5, 5, 246, 246);
        CGContextSetRGBFillColor(context, 0, 0, 1, 1);
        CGContextFillEllipseInRect(context, rect);
        
        CGContextRelease(context);
        
        m_imageData = [NSData dataWithBytesNoCopy:data length:byteCount freeWhenDone:YES];
        return description;
    }
    
    TextureDescription LoadPngImage(const string& file)
    {
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        
        NSLog(@"Loading PNG image %@...", fullPath);
        
        UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
        CGImageRef cgImage = uiImage.CGImage;
        
        CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        m_imageData = [NSData dataWithData:(NSData*) dataRef];
        CFRelease(dataRef);
        
        m_hasPvrHeader = false;
        
        TextureDescription description;
        description.Width = (int)CGImageGetWidth(cgImage);
        description.Height = (int)CGImageGetHeight(cgImage);
        bool hasAlpha = CGImageGetAlphaInfo(cgImage) != kCGImageAlphaNone;
        CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImage);
        switch (CGColorSpaceGetModel(colorSpace)) {
            case kCGColorSpaceModelMonochrome:
                description.Format = hasAlpha ? TextureFormatGrayAlpha : TextureFormatGray;
                break;
            case kCGColorSpaceModelRGB:
                description.Format = hasAlpha ? TextureFormatRgba : TextureFormatRgb;
                break;
            default:
                assert(!"Unsupported color space.");
                break;
        }
        description.BitsPerComponent = (int)CGImageGetBitsPerComponent(cgImage);
        
        return description;
    }

    TextureDescription LoadPvrImage(const string& file)
    {
//        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
//        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
//        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        
        NSString* fullPath = [NSString stringWithUTF8String:file.c_str()];
        
        m_imageData = [NSData dataWithContentsOfFile:fullPath];
        m_hasPvrHeader = true;
        PVR_Texture_Header* header = (PVR_Texture_Header*) [m_imageData bytes];
        bool hasAlpha = header->dwAlphaBitMask ? true : false;
        
        TextureDescription description;
        switch (header->dwpfFlags & PVRTEX_PIXELTYPE) {
            case OGL_RGB_565:
                description.Format = TextureFormat565;
                break;
            case OGL_RGBA_5551:
                description.Format = TextureFormat5551;
                break;
            case OGL_RGBA_4444:
                description.Format = TextureFormatRgba;
                description.BitsPerComponent = 4;
                break;
            case OGL_PVRTC2:
                description.Format = hasAlpha ? TextureFormatPvrtcRgba2 :
                TextureFormatPvrtcRgb2;
                break;
            case OGL_PVRTC4:
                description.Format = hasAlpha ? TextureFormatPvrtcRgba4 :
                TextureFormatPvrtcRgb4;
                break;
            default:
                assert(!"Unsupported PVR image.");
                break;
        }
        
        description.Width = header->dwWidth;
        description.Height = header->dwHeight;
        description.MipCount = header->dwMipMapCount;
        return description;
    }
    
    void* GetImageData()
    {
        if (!m_hasPvrHeader)
            return (void*) [m_imageData bytes];
        
        PVR_Texture_Header* header = (PVR_Texture_Header*) [m_imageData bytes];
        char* data = (char*) [m_imageData bytes];
        unsigned int headerSize = header->dwHeaderSize;
        return data + headerSize;
    }
    
    void UnloadImage()
    {
        m_imageData = 0;
    }
    
private:
    NSData* m_imageData;
    bool m_hasPvrHeader;

};


IResourceManager* CreateResourceManager()
{
    return new ResourcesManager();
}
