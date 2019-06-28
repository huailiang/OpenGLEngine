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
     MeshData* quad2 = nullptr;
    time_t start_time;
    float deltatime;
    
    void SystemInfo(int level)
    {
#ifdef DEBUG
        std::cout<<"OpenGL information"<<std::endl;
        std::cout<<"VENDOR:    "<< glGetString(GL_VENDOR)<<std::endl;
        std::cout<<"RENDERER:  "<< glGetString(GL_RENDERER)<<std::endl;
        std::cout<<"VERSION:   "<< glGetString(GL_VERSION)<<std::endl;
        std::cout<<"GLSL VER:  "<< glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
        if(level > 0)
        {
            int ext=0; glGetIntegerv(GL_NUM_EXTENSIONS,&ext);
            std::cout<<"EXTENSIONS "<< ext<<std::endl;
            loop(ext) std::cout<<i<<" "<<glGetStringi(GL_EXTENSIONS,i)<<std::endl;
            if(level > 2)
            {
                int num_formats;
                glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num_formats);
                std::cout<<"Texture extensions: "<<num_formats<<std::endl;
                int *formats = (int*)alloca(num_formats * sizeof(int));
                glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);
                loop(num_formats) std::cout<<i<<" 0x"<<hex<<formats[i]<<dec<<std::endl;
            }
        }
#endif
    }
    
    float GetRuntime()
    {
        timeb t;
        ftime(&t);
        if (start_time == 0) start_time = t.time;
        return t.time - start_time + t.millitm * 0.001f;
    }
    

    float GetDeltaTime()
    {
        return deltatime;
    }
    
    void OnApplicationQuit()
    {
        std::cout<<"engine will be quit"<<std::endl;
    }
    
    void OnApplicationPause(bool pause)
    {
        std::cout<<"engine pause "<<pause<<std::endl;
    }

    void* InitPlane(GLuint &vao, GLuint &vbo, Shader* shader)
    {
        if(plane==nullptr) plane = ReadMesh("plane","common");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        plane->ConfigAttribute();
        glBindVertexArray(0);
        if(shader) plane->Bind(shader);
        return plane;
    }

    void* InitCube(GLuint &vao, GLuint &vbo, Shader* shader)
    {
        if(cube == nullptr) cube = ReadMesh("cube","common");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);
        cube->ConfigAttribute();
        glBindVertexArray(0);
        if(shader) cube->Bind(shader);
        return cube;
    }

    // ndc position -> right_btm corner
    void* InitQuad(GLuint &vao, GLuint &vbo, Shader* shader)
    {
        if(quad == nullptr) quad = ReadMesh("quad","common");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        quad->ConfigAttribute();
        glBindVertexArray(0);
        if(shader) quad->Bind(shader);
        return quad;
    }
    
    // ndc position->[-1,1]
    void* InitFullQuad(GLuint &vao, GLuint &vbo, Shader* shader)
    {
        if(quad2 == nullptr) quad2 = ReadMesh("quad2","common");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        quad2->ConfigAttribute();
        glBindVertexArray(0);
        if(shader) quad2->Bind(shader);
        return quad2;
    }
    
    
    void SetPosition(glm::mat4& mat, glm::vec3& pos)
    {
        mat[3][0] = pos.x;
        mat[3][1] = pos.y;
        mat[3][2] = pos.z;
    }
    
    void error_stop(bool condition, const char* msg)
    {
        if (!condition) {
            std::cerr<<msg<<std::endl;
        }
        assert(condition);
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
        if (str.empty()) return 0;
        uint hash = 0;
        int seed = 5;
        loop(str.size()) hash = (hash << seed) + str[i] + hash;
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
