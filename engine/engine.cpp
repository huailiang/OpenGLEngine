//
//  common.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "engine.h"
#include <sys/timeb.h>
#include "std/util.h"

namespace engine
{

    uint SCR_WIDTH = 800;
    uint SCR_HEIGHT = 600;

    uint RENDER_WIDTH = 1600;
    uint RENDER_HEIGTH = 1200;
    uint DRAW_MODE = GL_TRIANGLES;
    
    MeshData* plane = nullptr;
    MeshData* cube = nullptr;
    MeshData* quad = nullptr;

    #ifdef _QT_EDIT_
    QTime q_time;
    #else
    time_t start_time;
    #endif

    void GlobalInit()
    {
    #ifdef _QT_EDIT_
        q_time.start();
    #endif
    }

    float GetRuntime()
    {
    #ifdef _QT_EDIT_
        return q_time.elapsed()*0.001f;
    #else
        timeb t;
        ftime(&t);
        if (start_time == 0) start_time = t.time;
        return t.time - start_time + t.millitm * 0.001f;
    #endif
    }


    void InitPlane(GLuint &vao, GLuint &vbo)
    {
        if(plane==nullptr) plane = ReadMesh("plane","common");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        plane->ConfigAttribute();
        glBindVertexArray(0);
    }

    void InitCube(GLuint &vao, GLuint &vbo)
    {
        if(cube == nullptr) cube = ReadMesh("cube","common");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);
        cube->ConfigAttribute();
        glBindVertexArray(0);
    }

    void InitQuad(GLuint &vao, GLuint &vbo)
    {
        if(quad == nullptr) quad = ReadMesh("quad","common");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        quad->ConfigAttribute();
        glBindVertexArray(0);
    }


    std::string Macro(const char* k1)
    {
        std::stringstream stream;
        MACRO(k1, NULL)
        return stream.str();
    }

    std::string Macro(const char* k1, const char* v1)
    {
        std::stringstream stream;
        MACRO(k1, v1)
        return stream.str();
    }

    std::string Macro(const char* k1, const char* v1, const char* k2, const char* v2)
    {
        std::stringstream stream;
        MACRO(k1, v1)
        MACRO(k2, v2)
        return stream.str();
    }

    std::string Macro(const char* k1, const void* v1, const char* k2, const void* v2, const char* k3, const char* v3)
    {
        std::stringstream stream;
        MACRO(k1, v1)
        MACRO(k2, v2)
        MACRO(k3, v3)
        return stream.str();
    }

    void SetRenderMode(const uint mode)
    {
        DRAW_MODE = mode;
    }

    void SetWindowSize(const int width, const int height)
    {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
        RENDER_WIDTH = width * 2;
        RENDER_HEIGTH = height * 2;
    }
    
    uint Hash(const std::string str)
    {
        if (str.empty()) {
            return 0;
        }
        uint hash =0;
        int seed = 5;
        for (int i=0; i<str.size(); i++) {
            short sc = (short)str[i];
            hash = (hash << seed) + str[i] + hash;
        }
        return hash;
    }

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
    
    void writevec3(std::ofstream& f, vec3& v)
    {
        f.write((char*)&(v.x),sizeof(float));
        f.write((char*)&(v.y),sizeof(float));
        f.write((char*)&(v.z),sizeof(float));
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

    void caltangent(const glm::vec3 pos1, const glm::vec3 pos2, const glm::vec3 pos3,
                    const glm::vec2 uv1, const glm::vec2 uv2, const glm::vec2 uv3,
                    glm::vec3* tan, glm::vec3* bit)
    {
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
        
        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        (*tan).x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        (*tan).y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        (*tan).z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        (*tan) = normalize(*tan);
        
        (*bit).x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        (*bit).y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        (*bit).z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        (*bit) = glm::normalize(*bit);
    }
    
    
}
