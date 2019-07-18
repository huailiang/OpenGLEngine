//
//  engine.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef engine_h
#define engine_h

#ifdef _QT_EDIT_
#include <QtOpenGL>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <stdio.h>
#include <regex>
#endif


#ifndef _GLES_
#ifndef _QT_EDIT_
#include <glad/glad.h>
#endif // _QT_EDIT_
#include <GLFW/glfw3.h>
#else
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif // __APPLE__
#endif // _GLES_

#ifdef ANDROID
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#endif

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "profile.h"

namespace engine
{

    const double PI = 3.14159265359;
    typedef unsigned int uint;

    extern uint SCR_WIDTH;
    extern uint SCR_HEIGHT;
    
    extern uint UI_WIDTH;
    extern uint UI_HEIGHT;

    //initial with sreen*2
    extern uint RENDER_WIDTH;
    extern uint RENDER_HEIGTH;

    extern uint DRAW_MODE;
    extern float deltatime;
    
    extern bool ENG_PAUSE;
    
    class Shader;

    #define WORKDIR "/Users/penghuailiang/Documents/projects/GL/OpenGLEngine/"


    #define DeclareSington(TYPE)  \
    private:    \
        TYPE() { }  \
        TYPE(const TYPE &); \
        TYPE& operator=(const TYPE &);  \
        static TYPE instance;   \
    public: \
        static TYPE* getInstance() { return &instance; } \


    #define SAFE_DELETE(ptr) \
    if(ptr != nullptr) \
    {delete ptr; ptr = nullptr;}
    
    
    #define DELETE_TEXTURE(texID)   \
    TexMgr::getInstance()->RemvTexture(texID); \
    glDeleteTextures(1, &texID);   \
    
    #define max(a,b) ((a)>(b)?(a):(b))
    
    #define min(a,b) ((a)<(b)?(a):(b))
    
    #undef foreach
    #define foreach(var, container) for( auto var = (container).begin(); var != (container).end(); ++var)

    #define loop(end_l) for (int i=0;i<end_l;++i)
    
    #define loop0i(end_l) for (size_t i=0;i<end_l;++i)
    
    #define loop0j(end_l) for (size_t j=0;j<end_l;++j)
    
    #define clamp(a_,b_,c_) min(max(a_,b_),c_)
    
    #define frac(a) (a-floor(a))
    
    
    const float zeroVertices[] = {0.0f,  0.0f};
    
    void SystemInfo(int level = 0);

    float GetRuntime();
    
    // will be auto called while application is quitting
    void OnApplicationQuit();
    
    // will be auto called while application is becoming active/inactive
    void OnApplicationPause(bool pause);

    void* InitPlane(GLuint *planeVAO, GLuint *planeVBO, Shader* shader = nullptr);

    void* InitCube(GLuint *cubeVAO, GLuint *cubeVBO, Shader* shader = nullptr);

    // 屏幕右下角 position
    void* InitQuad(GLuint *quadVAO, GLuint *quadVBO, Shader* shader = nullptr);
    
    // 全屏 position
    void* InitFullQuad(GLuint *quadVAO, GLuint *quadVBO, Shader* shader = nullptr);
    
    uint InitSpere(GLuint *vao, GLuint *vbo, GLuint *ebo);
    
    void SetPosition(glm::mat4& mat, glm::vec3& pos);
    
    void error_stop(bool condition, const char* msg);

    #define MACRO(k1, v1) \
        stream << "#define " << k1; \
        if(v1) stream << "  "<<v1;  \
        stream << std::endl;
    
    std::string Macro(const char* k1);

    std::string Macro(const char* k1, const char* v1);

    std::string Macro(const char* k1, const char* v1, const char* k2, const char* v2);

    std::string Macro(const char* k1, const void* v1, const char* k2, const void* v2, const char* k3, const char* v3);

    void SetRenderMode(const uint mode);

    void SetWindowSize(const int width, const int height);
    
    uint Hash(const std::string str);
    
    void caltangent(const glm::vec3 pos1, const glm::vec3 pos2, const glm::vec3 pos3,
                    const glm::vec2 uv1, const glm::vec2 uv2, const glm::vec2 uv3,
                    glm::vec3* tan, glm::vec3* bit);
    
    
}

#endif /* engine_h */
