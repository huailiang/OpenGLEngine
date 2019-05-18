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
    DeclareSington(SceneMgr)
    
private:
    ~SceneMgr()
    {
        LeaveScene();
        delete lb_scene1;
        delete lb_scene2;
        delete lb_scene3;
        delete lb_fps;
        lb_scene1 = NULL;
        lb_scene2 = NULL;
        lb_scene3 = NULL;
        lb_fps = NULL;
    }
    
    static void ClickScene(UIEvent* contex, void* arg)
    {
        Label* label = dynamic_cast<Label*>(contex);
        cout<<"change to scene: "<<label->getText()<<endl;
        int evtid = contex->evtid;
        instance.ChangeTo(TY_Scene1 + evtid);
    }

    
public:
    
    void Init()
    {
        ChangeTo(TY_Scene1);
        lb_scene1 = new Label(vec2(120,380), vec3(1.0f), 1, "Scene1", 0);
        lb_scene2 = new Label(vec2(120,320), vec3(1.0f), 1, "Scene2", 1);
        lb_scene3 = new Label(vec2(120,260), vec3(1.0f), 1, "Scene3", 2);
        lb_fps = new Label(vec2(740,580), vec3(1,0,0), 0.5f);
        lb_copy = new Label(vec2(670,20), vec3(1),0.4f);
        lb_scene1->RegistCallback(ClickScene, this);
        lb_scene2->RegistCallback(ClickScene, this);
        lb_scene3->RegistCallback(ClickScene, this);
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
        if(lb_fps)
        {
            lb_fps->setText("FPS: "+to_string_with_precision(1/delta,4));
        }
        if(lb_copy)
        {
            lb_copy->setText("@copyright (c) penghuailiang");
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
    Label *lb_scene1, *lb_scene2, *lb_scene3;
    Label *lb_fps, *lb_copy;
};

#endif /* scenemgr_h */
