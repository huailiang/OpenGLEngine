/*
 *  FilePath.cpp
 */

#include "FilePath.h"
#include <fstream>
#include <iostream>
#include <CoreFoundation/CoreFoundation.h>
#import <QuartzCore/QuartzCore.h>
#include "Helpers.h"
#include <CoreGraphics/CoreGraphics.h>
#include <CoreGraphics/CGContext.h>

typedef struct
#ifdef __APPLE__
__attribute__ ( ( packed ) )
#endif
{
    unsigned char IdSize, MapType, ImageType;
    unsigned short PaletteStart, PaletteSize;
    unsigned char  PaletteEntryDepth;
    unsigned short X, Y, Width, Height;
    unsigned char  ColorDepth, Descriptor;
} TGA_HEADER; // 18 byte


char* getsPath(const char *filename)
{
    char *ptr;
    std::string fnm(filename);
    if(fnm.find("/") == fnm.npos)
    {
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
        CFStringRef str = CFURLCopyFileSystemPath(resourcesURL, kCFURLPOSIXPathStyle);
        CFRelease(resourcesURL);
        ptr = new char[CFStringGetLength(str)+1];
        CFStringGetCString(str, ptr, FILENAME_MAX, kCFStringEncodingASCII);
        CFRelease(str);
        
        std::string res(ptr);
        res += std::string("/");
        res += std::string(filename);
        
        delete[] ptr;
        ptr = new char[res.length()+1];
        strcpy(ptr, res.c_str());
    }
    else
    {
        ptr = new char[fnm.length()+1];
        strcpy(ptr, fnm.c_str());
    }
    return ptr;
}

string getPath(const char *filename)
{
    std::string fnm(filename);
    return getPath(fnm);
}

string getPath(const string& filename)
{
    char *ptr;
    if(filename.find("/") == filename.npos)
    {
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
        CFStringRef str = CFURLCopyFileSystemPath(resourcesURL, kCFURLPOSIXPathStyle);
        CFRelease(resourcesURL);
        ptr = new char[CFStringGetLength(str)+1];
        CFStringGetCString(str, ptr, FILENAME_MAX, kCFStringEncodingASCII);
        CFRelease(str);
        
        std::string res(ptr);
        res += std::string("/");
        res += std::string(filename);
        
        delete[] ptr;
        ptr = new char[res.length()+1];
        strcpy(ptr, res.c_str());
    }
    else
    {
        ptr = new char[filename.length()+1];
        strcpy(ptr, filename.c_str());
    }
    string s(ptr);
    delete[] ptr;
    return s;
}

string getContentFromPath(const char *filepath)
{
    std::string buf("");
    std::string line("");
    std::ifstream in(filepath);
    if(in.fail()) return "";
    
    while(std::getline(in,line))
    {
        line += std::string("\n");
        buf += line;
    }
    char *ptr = new char[buf.length()];
    strcpy(ptr, buf.c_str());
    string s(ptr);
    delete[] ptr;
    return s;
}

const char* getsContentFromPath(const char *filepath)
{
    std::string buf("");
    std::string line("");
    std::ifstream in(filepath);
    
    if(in.fail())
        return "";
    while(std::getline(in,line))
    {
        line += std::string("\n");
        buf += line;
    }
    
    char *ptr = new char[buf.length()];
    strcpy(ptr, buf.c_str());
    return (const char*)ptr;
}

string getContentFromPath(const string& filepath)
{
    return getContentFromPath(filepath.c_str());
}

unsigned char* LoadImage(const char* filename, string extension, int *width, int *height)
{
    const char *filepath = getsPath(filename);
    CGDataProviderRef texturefiledata = CGDataProviderCreateWithFilename(filepath);
    if(!texturefiledata)  return NULL;
    
    bool Ispng = StringManipulator::IsEqual(extension,".png") == 0;
    bool IsJpg = StringManipulator::IsEqual(extension,".jpg") == 0;
    
    CGImageRef textureImage;
    if(IsJpg)
        textureImage = CGImageCreateWithJPEGDataProvider(texturefiledata, NULL, true, kCGRenderingIntentDefault);
    else if (Ispng)
        textureImage = CGImageCreateWithPNGDataProvider(texturefiledata, NULL, true, kCGRenderingIntentDefault);
    else {
        cerr<<"LoadImage: unsupported image type:"<<extension<<"\n";
        return NULL;
    }
    
    CGDataProviderRelease(texturefiledata);
    *width = (int)CGImageGetWidth(textureImage);
    *height = (int)CGImageGetHeight(textureImage);
    void *imageData = malloc(*height * *width * 4);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef textureContext = CGBitmapContextCreate(imageData, *width, *height, 8, 4 * *width, colorSpace,
                                                        kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    
    CGContextDrawImage(textureContext, CGRectMake(0, 0, *width, *height), textureImage);
    CFRelease(textureContext);
    CGImageRelease(textureImage);
    return (unsigned char*)imageData;
}

unsigned char* LoadImage(const string& filename, string extension, int *width, int *height)
{
    return LoadImage(filename.c_str(), extension, width, height);
}

size_t ReadFile(File *pFile, int bytesToRead, void *buffer)
{
    size_t bytesRead = 0;
    
    if(pFile == NULL) return bytesRead;
    
#ifdef ANDROID
    bytesRead = AAsset_read(pFile, buffer, bytesToRead);
#else
    bytesRead = fread(buffer, bytesToRead, 1, pFile);
#endif
    
    return bytesRead;
}


void FileClose(File *pFile)
{
    if(pFile != NULL)
    {
#ifdef ANDROID
        AAsset_close(pFile);
#else
        fclose(pFile);
        pFile = NULL;
#endif
    }
}


File* FileOpen(void *ioContext, const char *fileName)
{
    File *pFile = NULL;
    
#ifdef ANDROID
    if (ioContext != NULL)
    {
        AAssetManager *assetManager = (AAssetManager*)ioContext;
        pFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
    }
    
#else // __APPLE__
    fileName = getPath(fileName).c_str();
    pFile = fopen(fileName, "rb");
#endif

    return pFile;
}

unsigned char* LoadTGA(void *ioContext, const char *fileName, int *width, int *height,GLenum& format)
{
    unsigned char *buffer;
    File *fp;
    TGA_HEADER Header;
    size_t bytesRead;
    
    fp = FileOpen(ioContext, fileName);
    if (fp == NULL)
    {
        std::cout<< "esLoad FAILED to load: "<< fileName <<std::endl;
        return NULL;
    }
    
    bytesRead = ReadFile(fp, sizeof(TGA_HEADER), &Header);
    *width = Header.Width;
    *height = Header.Height;
    
    if (Header.ColorDepth == 8 || Header.ColorDepth == 24 || Header.ColorDepth == 32)
    {
        int bytesToRead = sizeof(char) * (*width) * (*height) * Header.ColorDepth / 8;
        buffer = (unsigned char*)malloc(bytesToRead);
        if (buffer)
        {
            bytesRead = ReadFile(fp, bytesToRead, buffer);
            FileClose(fp);
            return (buffer);
        }
        if(Header.ColorDepth == 8) format = GL_RED;
        else if(Header.ColorDepth == 24) format = GL_RGB;
        else format = GL_RGBA;
    }
    return NULL;
}

bool GetFormat(TextureDescription description,GLenum* format,int* bitsPerPixel)
{
    bool compressed = true;
    switch (description.Format)
    {
        case TextureFormatPvrtcRgba2:
            *bitsPerPixel = 2;
            *format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
            break;
        case TextureFormatPvrtcRgb2:
            *bitsPerPixel = 2;
            *format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
            break;
        case TextureFormatPvrtcRgba4:
            *bitsPerPixel = 4;
            *format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
            break;
        case TextureFormatPvrtcRgb4:
            *bitsPerPixel = 4;
            *format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            break;
        default:
            compressed = false;
            break;
    }
    return compressed;
}


IResourceManager* m_resourceManager;
unsigned char* LoadPvr(const char* filename, string ext, int* width, int* height, GLenum* format, GLint* level, int* bitsPerPixel)
{
    if (m_resourceManager==NULL) {
        m_resourceManager = CreateResourceManager();
    }
    TextureDescription description = m_resourceManager->LoadPvrImage(filename);
    *width = description.Width;
    *height = description.Height;
    *level = description.MipCount;
    GetFormat(description, format, bitsPerPixel);
    std::cout<<"format:"<<description.Format<<" w:"<<*width<<" h:"<<*height<<" format:"<<*format<<std::endl;
    return (unsigned char*)m_resourceManager->GetImageData();
}


void UnloadPvr()
{
    m_resourceManager->UnloadImage();
}

