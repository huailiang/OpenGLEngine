//
//  engine.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef common_h
#define common_h

#ifdef _QT_EDIT_
#include <QtOpenGL>
#endif
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <regex>

#ifndef _GLES_
#ifndef _QT_EDIT_
#include <glad/glad.h>
#endif // _QT_EDIT_
#include <GLFW/glfw3.h>
#else
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
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

namespace engine
{

    const double PI = 3.14159265359;

    extern unsigned int SCR_WIDTH;
    extern unsigned int SCR_HEIGHT;

    //initial with sreen*2
    extern unsigned int RENDER_WIDTH;
    extern unsigned int RENDER_HEIGTH;

    extern unsigned int DRAW_MODE;

    #ifdef _QT_EDIT_
    #define WORKDIR "/Users/penghuailiang/Documents/projects/GL/OpenGLEngine/" // Qt env
    extern QTime q_time;
    #else
    #define WORKDIR ""  //xcode wroking directory can be setted in Scheme
    #endif


    #define DeclareSington(TYPE)  \
    private:    \
        TYPE() { }  \
        TYPE(const TYPE &); \
        TYPE& operator=(const TYPE &);  \
        static TYPE instance;   \
    public: \
        static TYPE* getInstance() { return &instance; } \


    #define SAFE_DELETE(ptr) \
    if(ptr != NULL) \
    {delete ptr; ptr = NULL;}

    const float zeroVertices[] = {0.0f,  0.0f};

    void GlobalInit();

    float GetRuntime();

    void InitPlane(GLuint &planeVAO, GLuint &planeVBO);

    void InitCube(GLuint &cubeVAO, GLuint &cubeVBO);

    void InitQuad(GLuint &quadVAO, GLuint &quadVBO);

    #define MACRO(k1, v1) \
        stream << "#define " << k1; \
        if(v1) stream << "  "<<v1;  \
        stream << std::endl;

    std::string Macro(const char* k1);

    std::string Macro(const char* k1, const char* v1);

    std::string Macro(const char* k1, const char* v1, const char* k2, const char* v2);

    std::string Macro(const char* k1, const void* v1, const char* k2, const void* v2, const char* k3, const char* v3);

    void SetRenderMode(const unsigned int mode);

    void SetWindowSize(const int width, const int height);
    
    unsigned int Hash(const std::string str);
    
    void readv2(std::ifstream& f, glm::vec2& v);
    
    void readv3(std::ifstream& f, glm::vec3& v);
    
    void readstring(std::ifstream& f, std::string& str);
    
}

#endif /* common_h */
