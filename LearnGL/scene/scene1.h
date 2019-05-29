//
//  scene1.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene1_h
#define scene1_h

#include "scene.h"
#include "../std/terrain.h"


class Scene1 : public Scene
{
    
public:
   ~Scene1()
    {
        delete terrain;
        terrain = NULL;
        delete model;
        model = NULL;
        delete shader;
        shader = NULL;
        delete btn_normal;
        btn_normal = NULL;
        delete btn_debug;
        btn_debug = NULL;
    }
    
    int getType() { return TY_Scene1; }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(-1.0f));
    }
    
    void InitScene()
    {
        shader = new LightShader("model.vs", "model.fs");
        ApplyCamera(shader);
        nmShader = new Shader("gizmos/normal.vs","gizmos/pixel.fs","gizmos/normal.gs");
        ApplyCamera(nmShader);
        model = new Model("resources/objects/nanosuit/nanosuit.obj");
        terrain = new Terrain();
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        btn_normal = new UIButton(vec2(720, 360), vec3(1,1,0), 0.6f, "normal");
        btn_normal->RegistCallback(OnNormalClick, this);
        btn_debug = new UIButton(vec2(720, 330), vec3(1,1,0), 0.6f, "debug");
        btn_debug->RegistCallback(OnDebugClick, this);
    }
    
    void DrawShadow(Shader *depthShader)
    {
        Scene::DrawShadow(depthShader);
        terrain->DrawShadow(depthShader);
        model->DrawShadow(depthShader, camera, light, vec3(0.2f, -0.5f, -1.5f), vec3(0.12f), -16*timeValue);
    }
    
    void DrawScene()
    {
        terrain->Draw(camera, lightMatrix, light, depthMap);
        model->Draw(shader, camera, light, vec3(0.2f, -0.5f, -1.5f), vec3(0.12f), -16*timeValue);
        if(shownormal)
        {
            model->Draw(nmShader, camera, light, vec3(0.2f, -0.5f, -1.5f), vec3(0.12f), -16*timeValue);
        }
    }
    
    static void OnNormalClick(UIEvent* e, void* arg)
    {
        Scene1* scene = (Scene1*)(arg);
        scene->shownormal=!scene->shownormal;
    }
    
    static void OnDebugClick(UIEvent* e, void* arg)
    {
        Scene1* scene = (Scene1*)(arg);
        scene->debug=!scene->debug;
    }

    
    
private:
    UIButton* btn_normal, *btn_debug;
    Terrain* terrain;
    Model* model;
    LightShader* shader;
    Shader* nmShader;
    unsigned int texture1, texture2;
                           
public:
    bool shownormal;
};


#endif /* scene1_h */
