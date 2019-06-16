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
#include "terrain.h"
#include "avatar.h"
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
        SAFE_DELETE(btn_debug);
    }
    
    int getType() { return TY_Scene1; }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(-1.0f));
    }
    
    void InitScene()
    {
        shader = new LightShader("model.vs", "model.fs",nullptr, Macro("VERT_TYPE","0x0111"));
        ApplyCamera(shader);
        nmShader = new Shader("gizmos/normal.vs","gizmos/pixel.fs","gizmos/normal.gs");
        ApplyCamera(nmShader);
        terrain = new Terrain();
        nano = new Avatar("nanosuit",vec3(0.4f, -0.5f, -1.f), vec3(0.12f));
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
        nano->DrawShadow(depthShader, light);
    }
    
    void DrawScene()
    {
        terrain->Draw(camera, lightMatrix, light, depthMap);
        nano->Rotate(0.2f);
        nano->Draw(shader, light);
        if(shownormal)
        {
            nano->Draw(nmShader, light);
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
    LightShader* shader;
    Shader* nmShader;
    GLuint texture1, texture2;
    Avatar *nano;
public:
    bool shownormal;
};


#endif /* scene1_h */
