//
//  profile.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/9/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef profile_h
#define profile_h

#include <iomanip>
#include "common.h"

using namespace std;
using namespace glm;

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
                case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
                case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
                case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
                case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
                case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
                case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            std::cout << error << " | " << file << " (" << line << ")" << std::endl;
        }
        return errorCode;
    }
    #define glCheckError() glCheckError_(__FILE__, __LINE__)


    template <typename T>
    std::string to_string_with_precision(const T a_value, const int n = 6)
    {
        std::ostringstream out;
        out << std::setprecision(n) << a_value;
        return out.str();
    }

    void print_mat4(const mat4& mat)
    {
        cout.setf(ios::fixed);
        cout<<setprecision(2);
        cout<<mat[0][0]<<"\t"<<mat[0][1]<<"\t"<<mat[0][2]<<"\t"<<mat[0][3]<<endl;
        cout<<mat[1][0]<<"\t"<<mat[1][1]<<"\t"<<mat[1][2]<<"\t"<<mat[1][3]<<endl;
        cout<<mat[2][0]<<"\t"<<mat[2][1]<<"\t"<<mat[2][2]<<"\t"<<mat[2][3]<<endl;
        cout<<mat[3][0]<<"\t"<<mat[3][1]<<"\t"<<mat[3][2]<<"\t"<<mat[3][3]<<endl<<endl;
    }

    void print_mat4(const mat4& mat, const char* tag)
    {
        cout.setf(ios::fixed);
        cout<<setprecision(2);
        cout<<tag<<endl;
        cout<<mat[0][0]<<"\t"<<mat[0][1]<<"\t"<<mat[0][2]<<"\t"<<mat[0][3]<<endl;
        cout<<mat[1][0]<<"\t"<<mat[1][1]<<"\t"<<mat[1][2]<<"\t"<<mat[1][3]<<endl;
        cout<<mat[2][0]<<"\t"<<mat[2][1]<<"\t"<<mat[2][2]<<"\t"<<mat[2][3]<<endl;
        cout<<mat[3][0]<<"\t"<<mat[3][1]<<"\t"<<mat[3][2]<<"\t"<<mat[3][3]<<endl<<endl;
    }

    void print_vec3(const vec3& vec) 
    {
        cout<<"("<<vec.x<<", "<<vec.y<<", "<<vec.z<<")"<<endl<<endl;
    }

    void print_vec4(const vec4& vec)
    {
        cout<<"("<<vec.x<<", "<<vec.y<<", "<<vec.z<<", "<<vec.w<<")"<<endl<<endl;
    }

    void print_vec3(const vec3& vec, const char* tag)
    {
        cout<<tag<<": ("<<vec.x<<", "<<vec.y<<", "<<vec.z<<")"<<endl<<endl;
    }

    void print_vec4(const vec4& vec,const char* tag)
    {
        cout.setf(ios::fixed);
        cout<<setprecision(3);
        cout<<tag<<": ("<<vec.x<<", "<<vec.y<<", "<<vec.z<<", "<<vec.w<<")"<<endl<<endl;
    }

    void profile_test()
    {
        vec4 vec(1.0f,1.0f,0.0f,1.0f);
        print_vec4(vec,"debug1");
        mat4 trans;
        print_mat4(trans);
        trans= mat4(1.0f);
        print_mat4(trans,"ident");
        trans= translate(trans,vec3(1.0f, 1.0f, 0.0f));
        print_mat4(trans);
        vec = trans * vec;
        print_vec4(vec);
        mat4 m4 = ortho(0, 800, 0, 640, 0, 100);
        print_mat4(m4);
        m4 = perspective(radians(45.0f), 1920.0f/1080.0f, 2.0f, 10.0f);
        print_mat4(m4);
    }
    
}


#endif /* profile_h */
