//
//  texture.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/10/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include <glm/glm.hpp>
#include "asset.h"


using namespace glm;

namespace engine
{

    class Texture
    {
    public:
        
        GLuint textureID;
        
        Texture(const char* path, EXT ext, GLuint* texID= nullptr, bool flipY= true, int wrap = GL_REPEAT);
        
        // cubemap
        Texture(std::string path, GLuint* texID);
        
        ~Texture();
        
        void SetTextureFormat(GLenum target, GLint filter, GLint wrap, GLint align = 1);
        
        ivec2 GetShape();
        
        GLenum GetFormat(int format);
        
        GLuint LoadTexture(bool flipY, EXT ext, int wrap, bool gen_mipmap=true);
        
        GLuint LoadCubemap(std::string cubepath);
        
    private:
        
        unsigned char* RealLoad(int *width, int *height,EXT ext, GLenum* format, int* level, int* bitsPerPixel);
        
        void Free(unsigned char* data,EXT ext);
        
    private:
        
        int width, height;
        
        const char* path;
        
    };


}

#endif /* texture_h */
