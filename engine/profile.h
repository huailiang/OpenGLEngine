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
    
    std::ostream& operator<<(std::ostream& stream, glm::ivec2 vec);
    
    std::ostream& operator<<(std::ostream& stream, glm::ivec3 vec);
    
    std::ostream& operator<<(std::ostream& stream, glm::ivec4 vec);

    std::ostream& operator<<(std::ostream& stream, glm::vec2 vec);
    
    std::ostream& operator<<(std::ostream& stream, glm::vec3 vec);
    
    std::ostream& operator<<(std::ostream& stream, glm::vec4 vec);
    
    std::ostream& operator<<(std::ostream& stream, glm::mat2 mat);
    
    std::ostream& operator<<(std::ostream& stream, glm::mat3 mat);
    
    std::ostream& operator<<(std::ostream& stream, glm::mat4 mat);
    
    
}


#endif /* profile_h */
