//
//  Scene.cpp
//  GLESEngine
//
//  Created by 彭怀亮 on 6/1/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "Scene.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "skybox.h"
#include "label.h"
#include "avatar.h"
#include "asset.h"


using namespace engine;

unsigned int vao,vbo,texture1,texture2;
Shader* shader;
ESContext* esContext;
Camera* camera;
Light* light;
Skybox* skybox;
UILabel* label;


void Clean()
{
    glClearColor(0.2, 0.2, 0.2, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw(ESContext *esContext)
{
    Clean();
    
    vec3 cubePositions[] = { glm::vec3( 0.0f,  0.0f,  -2.0f), glm::vec3(2.0f,  1.0f, -4.0f) };
    shader->use();
    shader->setFloat("scale", 1);
    light->Apply(shader);
    camera->Attach(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(vao);
    for (unsigned int i = 0; i < 2; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = translate(model, cubePositions[i]);
        float angle = 64 * i * GetRuntime();
        model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
        model = glm::scale(model, vec3(0.5f));
        shader->setMat4("model", model);
        glDrawArrays(DRAW_MODE, 0, 36);
    }
    glBindVertexArray(0);
    if(skybox) skybox->Draw();
    if(label) label->drawText("hello, opengl es");
}

void ShutDown(ESContext* context)
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    SAFE_DELETE(shader);
    SAFE_DELETE(camera);
    SAFE_DELETE(light);
    SAFE_DELETE(skybox);
}

void SetWindowSize(ESContext *esContext)
{
    SetWindowSize(esContext->width, esContext->height);
    glViewport(0,0,esContext->width,esContext->height);
}

GLboolean initWindow()
{
#ifndef __APPLE__
    EGLConfig config;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    
    if(esContext == NULL) return GL_FALSE;
    
#ifdef ANDROID
    // For Android, get the width/height from the window rather than what the
    // application requested.
    esContext->width = ANativeWindow_getWidth (esContext->eglNativeWindow);
    esContext->height = ANativeWindow_getHeight (esContext->eglNativeWindow);
#else
    esContext->width = 400;
    esContext->height = 300;
#endif
    if (!WinCreate(esContext, "OpenGL ES Engine"))
    {
        return GL_FALSE;
    }
    
    esContext->eglDisplay = eglGetDisplay(esContext->eglNativeDisplay);
    if (esContext->eglDisplay == EGL_NO_DISPLAY)
    {
        return GL_FALSE;
    }
    
    // Initialize EGL
    if (!eglInitialize(esContext->eglDisplay, &majorVersion, &minorVersion))
    {
        return GL_FALSE;
    }
    
    EGLint numConfigs = 0;
    EGLint attribList[] =
    {
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_ALPHA_SIZE,     ( flags & ES_WINDOW_ALPHA ) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     ( flags & ES_WINDOW_DEPTH ) ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   ( flags & ES_WINDOW_STENCIL ) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, ( flags & ES_WINDOW_MULTISAMPLE ) ? 1 : 0,
        // if EGL_KHR_create_context extension is supported, then we will use
        // EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
        EGL_RENDERABLE_TYPE, GetContextRenderableType ( esContext->eglDisplay ),
        EGL_NONE
    };
    
    // Choose config
    if (!eglChooseConfig (esContext->eglDisplay, attribList, &config, 1, &numConfigs))
    {
        return GL_FALSE;
    }
    if (numConfigs < 1)
    {
        return GL_FALSE;
    }
    
    
#ifdef ANDROID
    // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
    EGLint format = 0;
    eglGetConfigAttrib ( esContext->eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format );
    ANativeWindow_setBuffersGeometry (esContext->eglNativeWindow, 0, 0, format);
#endif // ANDROID
    
    // Create a surface
    esContext->eglSurface = eglCreateWindowSurface(esContext->eglDisplay, config, esContext->eglNativeWindow, NULL);
    if (esContext->eglSurface == EGL_NO_SURFACE)
    {
        return GL_FALSE;
    }
    
    // Create a GL context
    esContext->eglContext = eglCreateContext(esContext->eglDisplay, config,EGL_NO_CONTEXT, contextAttribs );
    
    if(esContext->eglContext == EGL_NO_CONTEXT)
    {
        return GL_FALSE;
    }
    
    // Make the context current
    if (!eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface, esContext->eglSurface, esContext->eglContext))
    {
        return GL_FALSE;
    }
    
#endif // #ifndef __APPLE__
    
    return GL_TRUE;
}

bool InitScene(ESContext* context)
{
    esContext = context;
    initWindow();
    SystemInfo();
    
    camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
    skybox = new Skybox(camera, "mp_5dim");
    light = new DirectLight(vec3(1.0f), vec3(-1, 0, -2));
    TTFont::getInstance()->initial();
    label = new UILabel(vec2(30,560), vec3(1), 1);
    shader = new LightShader("light.vs","light.fs");
    shader->attach(light->getMacro().c_str());
    InitCube(vao, vbo, shader);
    shader->use();
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);
    camera->Attach(shader);
    Texture("textures/container", PVR, &texture1);
    Texture("textures/awesomeface", PNG, &texture2);
    context->drawFunc = Draw;
    context->shutdownFunc = ShutDown;
    context->updateWindow = SetWindowSize;
    return true;
}

