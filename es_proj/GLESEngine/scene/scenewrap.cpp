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
#include "IARInterface.h"

using namespace engine;

SceneMgr SceneMgr::instance;

ESContext* esContext;

float lastTime;

void init_engine()
{
    SystemInfo();
    TTFont::getInstance()->initial();
    SceneMgr::getInstance()->Init();
}

bool OpenCamera()
{
    auto scene = SceneMgr::getInstance()->current;
    return scene!= nullptr && scene->isARScene();
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
    if(scene->isARScene() && !ENG_PAUSE)
    {
        ARScene* sc = static_cast<ARScene*>(scene);
        sc->SetCameraFrame(frame);
    }
}

// ready for draw vr
void OnFrameDetect(ESContext *esContext,const std::vector<Transformation>& transforms)
{
    auto scene = SceneMgr::getInstance()->current;
    if(scene->isARScene() && !ENG_PAUSE)
    {
        ARScene* sc = static_cast<ARScene*>(scene);
        sc->DrawAR(transforms);
    }
}

void OnFrameInit(ESContext *esContext, float width, float height,const glm::mat3& intrinsic)
{
    auto scene = SceneMgr::getInstance()->current;
    if(scene->isARScene())
    {
        ARScene* sc = static_cast<ARScene*>(scene);
        sc->InitialVR(width, height, intrinsic);
    }
}

void OnProcessAR(ESContext *esContext, IARInterface* ar)
{
     SceneMgr::getInstance()->ptr_ar = ar;
}

bool InitScene(ESContext* context, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
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
    context->arProcess = OnProcessAR;
    return true;
}

