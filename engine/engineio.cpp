//
//  common.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "engineio.h"

namespace engine
{
    
    void readv2(std::ifstream& f, glm::vec2& v)
    {
        f.read((char*)&v.x,sizeof(float));
        f.read((char*)&v.y,sizeof(float));
    }
    
    void readv3(std::ifstream& f, glm::vec3& v)
    {
        f.read((char*)&(v.x),sizeof(float));
        f.read((char*)&(v.y),sizeof(float));
        f.read((char*)&(v.z),sizeof(float));
    }
    
    void readv3(std::ifstream& f, glm::ivec3& v)
    {
        f.read((char*)&(v.x),sizeof(int));
        f.read((char*)&(v.y),sizeof(int));
        f.read((char*)&(v.z),sizeof(int));
    }
    
    void readstring(std::ifstream& f, std::string& str)
    {
        size_t len;
        f.read((char*)&len, sizeof(size_t));
        if(len > 0)
        {
            char* temp = new char[len+1];
            f.read(temp, len);
            temp[len] = '\0';
            str = temp;
            delete []temp;
        }
    }
    
    void readarray(std::ifstream& f, float arr[], int num)
    {
        loop0i(num) f.read((char*)(arr+i),sizeof(float));
    }
    
    void readmat4(std::ifstream& f, glm::mat4& mat)
    {
        loop(4) loop0j(4) f.read((char*)&mat[i][(int)j],sizeof(float));
    }
    
    void readvec3(std::ifstream& f, glm::vec3& v)
    {
        f.read((char*)&(v.x),sizeof(float));
        f.read((char*)&(v.y),sizeof(float));
        f.read((char*)&(v.z),sizeof(float));
    }
    
    void readvec4(std::ifstream& f, glm::vec4& v)
    {
        f.read((char*)&(v.x),sizeof(float));
        f.read((char*)&(v.y),sizeof(float));
        f.read((char*)&(v.z),sizeof(float));
        f.read((char*)&(v.w),sizeof(float));
    }

    void writestring(ofstream& f, std::string str)
    {
        size_t len = str.size();
        f.write((char*)&len, sizeof(size_t));
        if(len > 0) f.write(str.c_str(), len);
    }
    
    void writevec2(std::ofstream& f, glm::vec2& v)
    {
        f.write((char*)&(v.x),sizeof(float));
        f.write((char*)&(v.y),sizeof(float));
    }
    
    void writevec3(std::ofstream& f, vec3& v)
    {
        f.write((char*)&(v.x),sizeof(float));
        f.write((char*)&(v.y),sizeof(float));
        f.write((char*)&(v.z),sizeof(float));
    }
    
    void writevec3(std::ofstream& f, ivec3& v)
    {
        f.write((char*)&(v.x),sizeof(int));
        f.write((char*)&(v.y),sizeof(int));
        f.write((char*)&(v.z),sizeof(int));
    }
    
    void writevec4(std::ofstream& f, vec4& v)
    {
        f.write((char*)&(v.x),sizeof(float));
        f.write((char*)&(v.y),sizeof(float));
        f.write((char*)&(v.z),sizeof(float));
        f.write((char*)&(v.w),sizeof(float));
    }
    
    void writearray(std::ofstream& f, float arr[], int num)
    {
        loop0i(num) f.write((char*)(arr+i),sizeof(float));
    }
    
    void writemat4(std::ofstream& f, mat4 mat)
    {
        loop(4) loop0j(4) f.write((char*)&mat[i][(int)j],sizeof(float));
    }

    
}
