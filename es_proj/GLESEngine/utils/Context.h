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
#include "BGRAVideoFrame.h"
#include "GeometryTypes.hpp"
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif


#define ES_WINDOW_RGB           0
#define ES_WINDOW_ALPHA         1
#define ES_WINDOW_DEPTH         2
#define ES_WINDOW_STENCIL       4
#define ES_WINDOW_MULTISAMPLE   8

typedef struct ESContext ESContext;

struct ESContext
{
    GLint width;
    
    GLint height;
    
#ifndef __APPLE__
    
    EGLNativeDisplayType eglNativeDisplay;
    
    EGLNativeWindowType eglNativeWindow;
    
    EGLDisplay eglDisplay;
    
    EGLContext eglContext;
    
    EGLSurface  eglSurface;
#endif
    
    void (*drawFunc) (ESContext*);
    void (*shutdownFunc) (ESContext*);
    void (*keyFunc) (ESContext *, unsigned char, int, int);
    void (*updateFunc) (ESContext *, float deltaTime);
    void (*updateWindow) (ESContext*);
    void (*pauseFunc)(ESContext*, bool);
    void (*tapFunc)(ESContext*, float, float);
    void (*frameReadyFunc)(ESContext*, const BGRAVideoFrame&);
    void (*frameDetectFunc)(ESContext*, const std::vector<Transformation>&);
    void (*frameInitFunc)(ESContext*, int width, int height, const Matrix33&);
};


#endif /* context.h */
