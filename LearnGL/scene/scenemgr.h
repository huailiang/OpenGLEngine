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
#include "../gui/uimgr.h"
#include "../gui/label.h"
#include "../gui/uievent.h"

class SceneMgr
{
private:
    SceneMgr() { }
    
    ~SceneMgr()
    {
        delete label1;
        delete label2;
        delete label3;
        label1 = NULL;
        label2 = NULL;
        label3 = NULL;
        if(current)
        {
            delete current;
            current = NULL;
        }
    }
    
    SceneMgr(const SceneMgr &);
    SceneMgr& operator=(const SceneMgr &);
    static SceneMgr instance;
    
    static void Test(UIEvent* contex)
    {
        Label* label = dynamic_cast<Label*>(contex);
        cout<<"change to scene: "<<label->getText()<<endl;
        int evtid = contex->evtid;
        instance.ChangeTo(TY_Scene1 + evtid);
    }

    
public:
    static SceneMgr* getInstance()
    {
        return &instance;
    }
    
    void Init()
    {
        ChangeTo(TY_Scene1);
        label1 = new Label(vec2(120,380), vec3(1.0f), 1, "Scene1", 0);
        label2 = new Label(vec2(120,320), vec3(1.0f), 1, "Scene2", 1);
        label3 = new Label(vec2(120,260), vec3(1.0f), 1, "Scene3", 2);
        label1->RegistCallback(Test);
        label2->RegistCallback(Test);
        label3->RegistCallback(Test);
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
    
    void ChangeTo(int type)
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
    Label *label1, *label2, *label3;
};

#endif /* scenemgr_h */
