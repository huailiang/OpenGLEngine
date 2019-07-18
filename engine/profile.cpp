//
//  profile.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/21/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "profile.h"

namespace engine
{
    
    GLenum glCheckError_(const char *file, int line)
    {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
                case GL_INVALID_ENUM:                  error = "GL_INVALID_ENUM"; break;
                case GL_INVALID_VALUE:                 error = "GL_INVALID_VALUE"; break;
                case GL_INVALID_OPERATION:             error = "GL_INVALID_OPERATION"; break;
#ifndef _GLES_
                case GL_STACK_OVERFLOW:                error = "GL_STACK_OVERFLOW"; break;
                case GL_STACK_UNDERFLOW:               error = "GL_STACK_UNDERFLOW"; break;
#endif
                case GL_OUT_OF_MEMORY:                 error = "GL_OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            std::cout << error << " | " << file << " (" << line << ")" << std::endl;
        }
        return errorCode;
    }
    
    std::ostream& operator<<(std::ostream& stream, glm::ivec2 c)
    {
        return stream<<" ("<<c.x<<", "<<c.y<<")";
    }
    
    std::ostream& operator<<(std::ostream& stream, glm::ivec3 c)
    {
        return stream<<" ("<<c.x<<", "<<c.y<<", "<<c.z<<")";
    }
    
    std::ostream& operator<<(std::ostream& stream, glm::ivec4 c)
    {
        return stream<<" ("<<c.x<<", "<<c.y<<", "<<c.z<<", "<<c.w<<")";
    }
    
    std::ostream& operator<<(std::ostream& stream, glm::vec2 c)
    {
        return stream<<" ("<<c.x<<", "<<c.y<<")";
    }
    
    std::ostream& operator<<(std::ostream& stream,glm::vec3 c)
    {
        return stream<<" ("<<c.x<<", "<<c.y<<", "<<c.z<<")";
    }
    
    std::ostream& operator<<(std::ostream& stream, glm::vec4 c)
    {
        return stream<<" ("<<c.x<<", "<<c.y<<", "<<c.z<<", "<<c.w<<")";
    }
    
    std::ostream& operator<<(std::ostream& stream, glm::mat2 mat)
    {
        stream<<setiosflags(ios::fixed)<<setprecision(2);
        return stream<<endl<< \
        mat[0][0]<<"\t"<<mat[0][1]<<endl<< \
        mat[1][0]<<"\t"<<mat[1][1]<<endl;
    }
    
    std::ostream& operator<<(std::ostream& stream, glm::mat3 mat)
    {
        stream<<setiosflags(ios::fixed)<<setprecision(2);
        return stream<<endl<< \
            mat[0][0]<<"\t"<<mat[0][1]<<"\t"<<mat[0][2]<<endl<< \
            mat[1][0]<<"\t"<<mat[1][1]<<"\t"<<mat[1][2]<<endl<< \
            mat[2][0]<<"\t"<<mat[2][1]<<"\t"<<mat[2][2]<<endl;
    }
    
    std::ostream& operator<<(std::ostream& stream, glm::mat4 mat)
    {
        stream<<setiosflags(ios::fixed)<<setprecision(2);
        return stream<<endl<< \
        mat[0][0]<<"\t"<<mat[0][1]<<"\t"<<mat[0][2]<<"\t"<<mat[0][3]<<endl<< \
        mat[1][0]<<"\t"<<mat[1][1]<<"\t"<<mat[1][2]<<"\t"<<mat[1][3]<<endl<< \
        mat[2][0]<<"\t"<<mat[2][1]<<"\t"<<mat[2][2]<<"\t"<<mat[2][3]<<endl<< \
        mat[3][0]<<"\t"<<mat[3][1]<<"\t"<<mat[3][2]<<"\t"<<mat[3][3]<<endl;
    }
    
}
