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
    }
    
    void DrawTerrain()
    {
        if(terrain==NULL)
        {
            terrain=new Terrain();
        }
        terrain->Draw(camera);
    }
    
    void DrawChar()
    {
        if(model==NULL)
        {
            model = new Model("resources/objects/nanosuit/nanosuit.obj");
        }
        float timeValue = glfwGetTime();
        model->Draw(shader, camera, light, vec3(0.2f, -0.5f, -1.5f), vec3(0.12f), -16*timeValue);
//        model->Draw(nmShader, camera, light, vec3(0.2f, -0.5f, -1.5f), vec3(0.12f), -16*timeValue);
    }
    
private:
    Terrain* terrain;
    Model* model;
    LightShader* shader;
    Shader* nmShader;
    unsigned int texture1, texture2;
};


#endif /* scene1_h */
