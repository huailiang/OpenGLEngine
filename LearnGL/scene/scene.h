//
//  scene1.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include "../common.h"
#include "../gui/uimgr.h"
#include "../gui/label.h"
#include "../gui/uievent.h"
#include "../std/camera.h"
#include "../std/light.h"

enum SceneType
{
    TY_Scene1,
    TY_Scene2,
    TY_Scene3,
};

//#define _SpotLight_

/*
 Scene build seqence:
    camera->
        light->
            scene
 */
class Scene
{
private:
    
    void ChangeScene(UIEvent* contex)
    {
    }
    

public:
    Scene()
    {
        if(camera==NULL)
        {
            camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
        }
        if(light == NULL)
        {
            InitLight();
            InitScene();
        }
    }
    
    virtual SceneType getType() = 0;
    
    virtual void InitLight() = 0;
    
    virtual void InitScene() = 0;
    
    virtual void DrawUI()
    {
        Label label1(vec2(120,20), vec3(1.0f), 1, "Scene1");
        Label label2(vec2(120,80), vec3(1.0f), 1, "Scene2");
        Label label3(vec2(120,140),vec3(1.0f), 1, "Scene3");
    }
    
    virtual void DrawTerrain() = 0;
    
    void DrawChar() { }
  
    
protected:
    Camera* camera;
    Light* light;
};


#endif /* scene_h */
