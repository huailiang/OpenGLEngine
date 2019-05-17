//
//  scene1.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene1_h
#define scene1_h

#include "scene.h"
#include "../terrain.h"


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
        delete lb_normal;
        lb_normal = NULL;
    }
    
    int getType()
    {
        return TY_Scene1;
    }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,0.0f,0.0f));
    }
    
    void InitScene()
    {
        shader = new LightShader("shader/model.vs", "shader/model.fs");
        nmShader = new Shader("shader/normal.vs","shader/normal.fs","shader/normal.gs");
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        lb_normal = new Label(vec2(580, 360), vec3(1,1,0), 0.6f, "normal");
        lb_normal->RegistCallback(OnClick, this);
    }
    
    
    void DrawScene()
    {
        Scene::ClearScene();
        if(terrain==NULL)
        {
            terrain=new Terrain();
        }
        terrain->Draw(camera);

        if(model==NULL)
        {
            model = new Model("resources/objects/nanosuit/nanosuit.obj");
        }
        float timeValue = glfwGetTime();
        model->Draw(shader, camera, light, vec3(0.2f, -0.5f, -1.5f), vec3(0.12f), -16*timeValue);
        if(shownormal)
        {
            model->Draw(nmShader, camera, light, vec3(0.2f, -0.5f, -1.5f), vec3(0.12f), -16*timeValue);
        }
    }
    
    
    static void OnClick(UIEvent* e, void* arg)
    {
        Scene1* scene = (Scene1*)(arg);
        scene->shownormal=!scene->shownormal;
    }
    
    
private:
    Label* lb_normal;
    Terrain* terrain;
    Model* model;
    LightShader* shader;
    Shader* nmShader;
    unsigned int texture1, texture2;
                           
public:
    bool shownormal;

};


#endif /* scene1_h */
