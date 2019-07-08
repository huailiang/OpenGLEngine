//
//  engineio.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef engineio_h
#define engineio_h

#ifdef _QT_EDIT_
#include <QtOpenGL>
#endif
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <stdio.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//#include <OpenGL/glext.h>

#include "profile.h"

namespace engine
{    
    void readv2(std::ifstream& f, glm::vec2& v);
    
    void readv3(std::ifstream& f, glm::vec3& v);
    
    void readv3(std::ifstream& f, glm::ivec3& v);
    
    void readstring(std::ifstream& f, std::string& str);
    
    void readarray(std::ifstream& f, float arr[], int num);
    
    void readmat4(std::ifstream& f, glm::mat4& mat);
    
    void readvec3(std::ifstream& f, glm::vec3& v);
    
    void readvec4(std::ifstream& f, glm::vec4& v);
    
    void writestring(std::ofstream& f, std::string str);
    
    void writevec2(std::ofstream& f, glm::vec2& v);
    
    void writevec3(std::ofstream& f, glm::vec3& v);
    
    void writevec3(std::ofstream& f, glm::ivec3& v);
    
    void writevec4(std::ofstream& f, glm::vec4& v);
    
    void writearray(std::ofstream& f, float arr[], int num);
    
    void writemat4(std::ofstream& f, glm::mat4 mat);
    
}

#endif /* engineio_h */
