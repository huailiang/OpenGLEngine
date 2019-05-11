//
//  texture.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/10/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION

using namespace glm;


class TTexture
{
public:
    unsigned int textureID;
    
    TTexture(const char* path)
    {
        TTexture(path, NULL);
    }
    
    TTexture(const char* path, unsigned int* texID)
    {
        this->path=path;
        *texID = LoadTexture();
    }
    
    unsigned int LoadTexture()
    {
        stbi_set_flip_vertically_on_load(true); // flip y
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *data = stbi_load(path, &width, &height, &format, 0);
        if (data)
        {
            GLenum glformat = GetFormat();
            glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, glformat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
        }
        stbi_image_free(data);
        return textureID;
    }
    
    ivec2 GetShape()
    {
        return ivec2(width,height);
    }
    
    GLenum GetFormat()
    {
        GLenum glformat;
        if (this->format == 1)
            glformat = GL_RED;
        else if (this->format == 3)
            glformat = GL_RGB;
        else if (this->format == 4)
            glformat = GL_RGBA;
        else
        {
            glformat = GL_RED;
            std::cout<<"texture is not support the format"<<this->format<<std::endl;
        }
        return glformat;
    }
    
private:
    int width, height, format;
    const char* path;
};



#endif /* texture_h */
