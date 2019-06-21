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
#include "engine.h"

using namespace std;
using namespace glm;

namespace engine
{

    GLenum glCheckError_(const char *file, int line);
    #define glCheckError() glCheckError_(__FILE__, __LINE__)


    template <typename T>
    std::string to_string_with_precision(const T a_value, const int n = 6)
    {
        std::ostringstream out;
        out << std::setprecision(n) << a_value;
        return out.str();
    }

    void print_mat4(const mat4& mat);

    void print_mat4(const mat4& mat, const char* tag);
    
    void print_vec3(const vec3& vec);
    
    void print_vec4(const vec4& vec);
    
    void print_vec3(const vec3& vec, const char* tag);
    
    void print_vec4(const vec4& vec,const char* tag);
    
    void profile_test();
}


#endif /* profile_h */
