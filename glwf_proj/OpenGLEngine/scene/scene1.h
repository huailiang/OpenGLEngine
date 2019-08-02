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
#include "terrain.hpp"
#include "avatar.hpp"
#include "util.h"

class Scene1 : public Scene
{
    
public:
    
   ~Scene1()
    {
        SAFE_DELETE(nano);
        SAFE_DELETE(terrain);
        SAFE_DELETE(shader);
        SAFE_DELETE(btn_normal);
    }
    
    int getType() { return TY_Scene1; }
    
    void InitLight()
    {
        light = new DirectLight(color_white, vec3(-3.0f));
    }
    
    void InitScene()
    {
        shader = new LightShader("model.vs", "model.fs");
        nmShader = new Shader("gizmos/normal.vs","gizmos/pixel.fs","gizmos/normal.gs");
        terrain = new Terrain();
        nano = new Avatar("nanosuit",vec3(0.4f, -0.5f, -1.f), vec3(0.12f));
        nano->BindVert(shader);
        nano->BindVert(nmShader);
        shader->compile();
        nmShader->compile();
        ApplyCamera(shader);
        ApplyCamera(nmShader);
    }
    
    void InitShadow()
    {
        shadow = new GeneralShadow(SHADOW_WIDTH, terrain->shader);
        GeneralShadow *gShadow = static_cast<GeneralShadow*>(shadow);
        debugTexID = gShadow->map;
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        btn_normal = new UIButton(vec2(720, 360), vec3(1,1,0), 0.6f, "normal");
        btn_normal->RegistCallback(OnNormalClick, this);
    }
    
    void DrawShadow()
    {
        Scene::DrawShadow();
        CulLightMatrix(near_plane, far_plane);
        GeneralShadow *gShadow = static_cast<GeneralShadow*>(shadow);
        gShadow->BindFbo(lightMatrix);
        ClearDepth();
        nano->Draw(shadow->getShader(), light, camera);
    }
    
    void DrawScene()
    {
        mat4 mtx[] = { lightMatrix };
        terrain->Draw(camera, mtx, light, shadow);
        nano->Rotate(0.2f);
        nano->Draw(shader, light, camera);
        if(shownormal) nano->Draw(nmShader, light, camera);
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
    UIButton* btn_normal = nullptr;
    Terrain* terrain = nullptr;
    LightShader* shader= nullptr;
    Shader* nmShader = nullptr;
    GLuint texture1, texture2;
    Avatar *nano = nullptr;
    
public:
    bool shownormal;
};


#endif /* scene1_h */
