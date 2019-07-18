//
//  Scene.cpp
//  GLESEngine
//
//  Created by 彭怀亮 on 6/1/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "scenewrap.h"
#include "scenemgr.h"
#include "BGRAVideoFrame.h"
#include "GeometryTypes.hpp"

using namespace engine;

SceneMgr SceneMgr::instance;

ESContext* esContext;
float lastTime;

void Clean()
{
    glClearColor(0.2, 0.2, 0.2, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void init_engine()
{
    SystemInfo();
    TTFont::getInstance()->initial();
    SceneMgr::getInstance()->Init();
}

bool OpenCamera()
{
    auto scene = SceneMgr::getInstance()->current;
    return scene!= nullptr && scene->isVRScene();
}


void Draw(ESContext *esContext)
{
    float timeValue = GetRuntime();
    deltatime  = timeValue-lastTime;
    lastTime= timeValue;
    SceneMgr::getInstance()->Draw(deltatime);
    UIManager::getInstance()->Draw();
}

void ShutDown(ESContext* context)
{
    SceneMgr::getInstance()->LeaveScene();
    OnApplicationQuit();
}

void OnPause(ESContext *esContext,bool pause)
{
    OnApplicationPause(pause);
}

void SetWindowSize(ESContext *esContext)
{
    SetWindowSize(esContext->width, esContext->height);
    glViewport(0,0,esContext->width,esContext->height);
}


void OnClickTriger(ESContext *esContext, float x, float y)
{
    float xx = UI_WIDTH  * x;
    float yy = UI_HEIGHT * y;
    EventMgr::getInstance()->DoTriger(xx, yy);
}

void OnFrameReady(ESContext *esContext,const BGRAVideoFrame& frame)
{
    auto scene = SceneMgr::getInstance()->current;
    if(scene->isVRScene() && !ENG_PAUSE)
    {
        Scene6* sc = static_cast<Scene6*>(scene);
        sc->SetCameraFrame(frame);
    }
}

// ready for draw vr
void OnFrameDetect(ESContext *esContext,const std::vector<Transformation>& transforms)
{
    auto scene = SceneMgr::getInstance()->current;
    if(scene->isVRScene() && !ENG_PAUSE)
    {
        Scene6* sc = static_cast<Scene6*>(scene);
        sc->DrawAR(transforms);
    }
}

void OnFrameInit(ESContext *esContext, float width, float height,const Matrix33& intrinsic)
{
    auto scene = SceneMgr::getInstance()->current;
    if(scene->isVRScene())
    {
        Scene6* sc = static_cast<Scene6*>(scene);
        sc->InitialVR(width, height, intrinsic);
    }
}

bool InitScene(ESContext* context)
{
    esContext = context;
    init_engine();
    
    context->drawFunc = Draw;
    context->shutdownFunc = ShutDown;
    context->pauseFunc =  OnPause;
    context->updateWindow = SetWindowSize;
    context->tapFunc = OnClickTriger;
    context->frameReadyFunc = OnFrameReady;
    context->frameDetectFunc = OnFrameDetect;
    context->frameInitFunc = OnFrameInit;
    return true;
}

