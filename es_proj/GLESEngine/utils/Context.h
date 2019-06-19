//
//  context.h
//  GLESEngine
//
//  Created by 彭怀亮 on 6/2/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef context_h
#define context_h

#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

typedef struct ESContext ESContext;

struct ESContext
{
    /// Window width
    GLint       width;
    
    /// Window height
    GLint       height;
    
#ifndef __APPLE__
    /// Display handle
    EGLNativeDisplayType eglNativeDisplay;
    
    /// Window handle
    EGLNativeWindowType  eglNativeWindow;
    
    /// EGL display
    EGLDisplay  eglDisplay;
    
    /// EGL context
    EGLContext  eglContext;
    
    /// EGL surface
    EGLSurface  eglSurface;
#endif
    
    void (*drawFunc) (ESContext*);
    void (*shutdownFunc) (ESContext*);
    void (*keyFunc) (ESContext *, unsigned char, int, int);
    void (*updateFunc) (ESContext *, float deltaTime);
    void (*updateWindow) (ESContext*);
};


#endif /* context.h */
