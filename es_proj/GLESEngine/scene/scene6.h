//
//  scene6.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/26/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene6_h
#define scene6_h

#include "scene.h"

class Scene6 : public Scene
{
    
public:
    
    ~Scene6()
    {
        SAFE_DELETE(shader);
        SAFE_DELETE(btn);
    }
    
    int getType() { return TY_Scene6; }
    
    std::string getSkybox() { return ""; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,16.0f); }
    
    static void OnClick(engine::UIEvent* e, void* arg)
    {
        Scene6* scene = (Scene6*)(arg);
        int evtid = e->evtid;
        scene->Click(evtid);
    }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(0.0f,0.0f,-2.0f));
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        btn = new engine::UIButton(vec2(660, 300), vec3(1,1,0), 0.6f, "  pose  ",0);
        btn->RegistCallback(OnClick, this);
    }
    
    void InitScene()
    {
        shader = new LightShader("model.vs", "model.fs");
        shader->compile();
        ApplyCamera(shader);
    }

    void DrawScene()
    {
    }
    
    void Click(int eid)
    {
    }
    
    
private:
    LightShader* shader;
    engine::UIButton *btn;
};


#endif /* scene6_h */
