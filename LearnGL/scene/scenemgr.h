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
public:
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
            current->DrawUI();
            current->DrawChar();
            current->DrawTerrain();
        }
    }
    
    void ChangeTo(SceneType type)
    {
        if(type == TY_Scene1)
        {
            ChangeTo(new Scene1());
        }
        if(type == TY_Scene2)
        {
            ChangeTo(new Scene2());
        }
        if(type == TY_Scene3)
        {
            ChangeTo(new Scene3());
        }
    }

    
private:
    Scene *current;
    
};

#endif /* scenemgr_h */
