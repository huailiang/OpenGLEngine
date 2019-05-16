//
//  scenemgr.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scenemgr_h
#define scenemgr_h

#include "scene.h"
#include "scene1.h"
#include "scene2.h"
#include "scene3.h"


class SceneMgr
{
private:
    SceneMgr() { }
    ~SceneMgr() { }
    SceneMgr(const SceneMgr &);
    SceneMgr& operator=(const SceneMgr &);
    static SceneMgr instance;
    
public:
    static SceneMgr* getInstance()
    {
        return &instance;
    }
    
    ~SceneMgr() { LeaveScene(); }
    
    void Init()
    {
        ChangeTo(TY_Scene1);
    }
    
    void LeaveScene()
    {
        if(current)
        {
            delete current;
            current = NULL;
        }
    }
    
    bool ChangeTo(Scene* scene)
    {
        if(current &&current->getType() == scene->getType())
        {
            return false;
        }
        LeaveScene();
        if(scene)
        {
            current = scene;
        }
        return true;
    }
    
    void Draw(float delta)
    {
        if(current)
        {
            current->DrawScene();
        }
    }
    
    void ChangeTo(SceneType type)
    {
        Scene* sc = NULL;
        if(type == TY_Scene1)
        {
            sc = new Scene1();
        }
        if(type == TY_Scene2)
        {
            sc = new Scene2();
        }
        if(type == TY_Scene3)
        {
            sc =new Scene3();
        }
        if(sc)
        {
            sc->Initial();
            ChangeTo(sc);
        }
    }
    
    void ProcessKeyboard(GLFWwindow *window, float deltatime)
    {
        if(current)
        {
            current->ProcessKeyboard(window, deltatime);
        }
    }
    
    void ProcessMouseMove(double xoffset,double yoffset)
    {
        if(current)
        {
            current->ProcessMouseMove(xoffset, yoffset);
        }
    }
    
    void ProcessMouseScroll(double xoffset,double yoffset)
    {
        if(current)
        {
            current->ProcessMouseScroll(xoffset, yoffset);
        }
    }


    
private:
    Scene *current;
    
};

#endif /* scenemgr_h */
