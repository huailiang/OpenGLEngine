/*
 *  FilePath.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
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
    unsigned char  IdSize,
    MapType,
    ImageType;
    unsigned short PaletteStart,
    PaletteSize;
    unsigned char  PaletteEntryDepth;
    unsigned short X,
    Y,
    Width,
    Height;
    unsigned char  ColorDepth,
    Descriptor;
    
} TGA_HEADER;


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
    
    string s(ptr);
    delete[] ptr;
    return s;
}

string getPath(const string& filename)
{
    return getPath(filename.c_str());
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

char* LoadImage(const char* filename, int *width, int *height)
{
    const char *filepath = getsPath(filename);
    CGDataProviderRef texturefiledata = CGDataProviderCreateWithFilename(filepath);
    
    if(!texturefiledata)   return NULL;
    
    string extension;
    StringManipulator::GetExtensitonType(filename,extension);
    StringManipulator::ToLowerCase(extension);
    bool Ispng = StringManipulator::IsEqual(extension,"png") == 0;
    bool IsJpg = StringManipulator::IsEqual(extension,"jpg") == 0;
    bool IsTga = StringManipulator::IsEqual(extension,"tga") == 0;
    
    CGImageRef textureImage;
    if(IsJpg)
        textureImage = CGImageCreateWithJPEGDataProvider(texturefiledata, NULL, true, kCGRenderingIntentDefault);
    else if (Ispng)
        textureImage = CGImageCreateWithPNGDataProvider(texturefiledata, NULL, true, kCGRenderingIntentDefault);
    else if(IsTga)
        return LoadTGA(NULL, filename, width, height);
    else {
        cerr<<"LoadPng: unsupported image type:"<<extension<<"\n";
        return NULL;
    }
    
    CGDataProviderRelease(texturefiledata);
    *width = (int)CGImageGetWidth(textureImage);
    *height = (int)CGImageGetHeight(textureImage);
    void *imageData = malloc(*height * *width * 4);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef textureContext = CGBitmapContextCreate(imageData,*width, *height, 8, 4 * *width, colorSpace,
                                                        kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    
    CGContextDrawImage(textureContext, CGRectMake(0, 0, *width, *height), textureImage);
    CFRelease(textureContext);
    CGImageRelease(textureImage);
    return (char*)imageData;
}
char* LoadImage(const string& filename, int *width, int *height)
{
    return LoadImage(filename.c_str(), width, height);
}

size_t ReadFile(esFile *pFile, int bytesToRead, void *buffer)
{
    size_t bytesRead = 0;
    
    if ( pFile == NULL )
    {
        return bytesRead;
    }
    
#ifdef ANDROID
    bytesRead = AAsset_read ( pFile, buffer, bytesToRead );
#else
    bytesRead = fread ( buffer, bytesToRead, 1, pFile );
#endif
    
    return bytesRead;
}


void FileClose(esFile *pFile)
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


esFile* FileOpen (void *ioContext, const char *fileName)
{
    esFile *pFile = NULL;
    
#ifdef ANDROID
    if ( ioContext != NULL )
    {
        AAssetManager *assetManager = ( AAssetManager * ) ioContext;
        pFile = AAssetManager_open ( assetManager, fileName, AASSET_MODE_BUFFER );
    }
    
#else
#ifdef __APPLE__
    fileName = getPath(fileName).c_str();
#endif
    
    pFile = fopen(fileName, "rb");
#endif
    
    return pFile;
}

char* LoadTGA(void *ioContext, const char *fileName, int *width, int *height)
{
    char *buffer;
    esFile *fp;
    TGA_HEADER  Header;
    size_t bytesRead;
    
    fp = FileOpen(ioContext, fileName);
    if (fp == NULL)
    {
        std::cout<< "esLoadTGA FAILED to load : "<< fileName <<std::endl;
        return NULL;
    }
    
    bytesRead = ReadFile(fp, sizeof ( TGA_HEADER ), &Header);
    *width = Header.Width;
    *height = Header.Height;
    
    if (Header.ColorDepth == 8 || Header.ColorDepth == 24 || Header.ColorDepth == 32)
    {
        int bytesToRead = sizeof ( char ) * ( *width ) * ( *height ) * Header.ColorDepth / 8;
        buffer = ( char * ) malloc ( bytesToRead );
        if (buffer)
        {
            bytesRead = ReadFile ( fp, bytesToRead, buffer );
            FileClose(fp);
            return (buffer);
        }
    }
    return NULL;
}


