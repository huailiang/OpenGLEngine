//
//  scenemgr.h
//  OpenGLEngine
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
#include "scene4.h"

class SceneMgr
{
    DeclareSington(SceneMgr)
    
private:
    ~SceneMgr()
    {
        LeaveScene();
        SAFE_DELETE(lb_scene1);
        SAFE_DELETE(lb_scene2);
        SAFE_DELETE(lb_scene3);
        SAFE_DELETE(lb_scene4);
        SAFE_DELETE(lb_fps);
    }
    
    static void ClickScene(UIEvent* contex, void* arg)
    {
        UILabel* label = dynamic_cast<UILabel*>(contex);
        cout<<"change to scene: "<<label->getText()<<endl;
        int evtid = contex->evtid;
        instance.ChangeTo(TY_Scene1 + evtid);
    }

    
public:
    
    void Init()
    {
        ChangeTo(TY_Scene2);
        lb_scene1 = new UILabel(vec2(60,380), vec3(1), 1, "Scene1", TY_Scene1);
        lb_scene2 = new UILabel(vec2(60,330), vec3(1), 1, "Scene2", TY_Scene2);
        lb_scene3 = new UILabel(vec2(60,280), vec3(1), 1, "Scene3", TY_Scene3);
        lb_scene4 = new UILabel(vec2(60,230), vec3(1), 1, "Scene4", TY_Scene4);
        lb_fps = new UILabel(vec2(740,580), vec3(1,0,0), 0.5f);
        lb_copy = new UILabel(vec2(20), vec3(1), 0.4f);
        lb_scene1->RegistCallback(ClickScene, this);
        lb_scene2->RegistCallback(ClickScene, this);
        lb_scene3->RegistCallback(ClickScene, this);
        lb_scene4->RegistCallback(ClickScene, this);
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
            std::cout<<"You are already enter scene "<<current->getType()<<std::endl;
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
            current->DrawScenes();
        }
        lb_fps->setText("FPS: "+to_string_with_precision(1/delta,4));
        lb_copy->setText("@copyright (c) penghuailiang");
    }
    
    void ChangeTo(int type)
    {
        Scene* scene = NULL;
        if(type == TY_Scene1)   scene = new Scene1();
        if(type == TY_Scene2)   scene = new Scene2();
        if(type == TY_Scene3)   scene =new Scene3();
        if(type == TY_Scene4)   scene =new Scene4();
        if(scene)
        {
            scene->Initial();
            ChangeTo(scene);
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
    UILabel *lb_scene1, *lb_scene2, *lb_scene3, *lb_scene4;
    UILabel *lb_fps, *lb_copy;
};

#endif /* scenemgr_h */
