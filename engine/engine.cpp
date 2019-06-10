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

    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;

    unsigned int RENDER_WIDTH = 1600;
    unsigned int RENDER_HEIGTH = 1200;
    unsigned int DRAW_MODE = GL_TRIANGLES;
    
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

    void SetRenderMode(const unsigned int mode)
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
    
    unsigned int Hash(const std::string str)
    {
        if (str.empty()) {
            return 0;
        }
        unsigned int hash =0;
        int seed = 5;
        for (int i=0; i<str.size(); i++) {
            hash = (hash << seed) + (short)str[i] + hash;
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
    
    
}
