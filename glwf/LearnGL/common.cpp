//
//  common.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "common.h"
#include <sys/timeb.h>

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

unsigned int RENDER_WIDTH = 1600;
unsigned int RENDER_HEIGTH = 1200;
unsigned int DRAW_MODE = GL_TRIANGLES;

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


void InitPlane(unsigned int &planeVAO, unsigned int &planeVBO)
{
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
}

void InitCube(unsigned int &cubeVAO, unsigned int &cubeVBO)
{
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void InitQuad(unsigned int &quadVAO, unsigned int &quadVBO)
{
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
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

void SetRenderMode(unsigned int mode)
{
    DRAW_MODE = mode;
}

void SetWindowSize(int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    RENDER_WIDTH = width * 2;
    RENDER_HEIGTH = height * 2;
}
