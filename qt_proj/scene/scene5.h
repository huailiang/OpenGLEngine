//
//  scene5.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/26/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene5_h
#define scene5_h

#include "scene.h"

class Scene5 : public Scene
{
    
public:
    
    ~Scene5()
    {
        SAFE_DELETE(halo);
        SAFE_DELETE(shader);
        SAFE_DELETE(btn1);
        SAFE_DELETE(btn2);
        SAFE_DELETE(btn3);
        SAFE_DELETE(btn4);
        SAFE_DELETE(btn5);
        SAFE_DELETE(lod1);
        SAFE_DELETE(lod2);
        SAFE_DELETE(lod3);
        SAFE_DELETE(lod4);
        SAFE_DELETE(lod5);
    }
    
    int getType() { return TY_Scene5; }
    
    std::string getSkybox() { return "mp_5dim"; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,16.0f); }
    
    void OnClick(engine::UIObject* e)
    {
        int evtid = e->uid;
        Click(evtid);
    }
    
    void OnLodClick(engine::UIObject* e)
    {
        int evtid = e->uid;
        LodSelect(evtid);
    }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(0.0f,0.0f,-2.0f));
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        auto f1 = Bindfunc(Scene5::OnClick);
        auto f2 = Bindfunc(Scene5::OnLodClick);
        btn1 = new engine::UIButton(vec2(660, 360), vec3(1,1,0), 0.6f, "  pose  ",0);
        btn1->RegistCallback(f1);
        btn2 = new engine::UIButton(vec2(660, 330), vec3(1,1,0), 0.6f, "   idle   ",1);
        btn2->RegistCallback(f1);
        btn3 = new engine::UIButton(vec2(660, 300), vec3(1,1,0), 0.6f, " pause ",2);
        btn3->RegistCallback(f1);
        btn4 = new engine::UIButton(vec2(660, 270), vec3(1,1,0), 0.6f, "resume",3);
        btn4->RegistCallback(f1);
        btn5 = new engine::UIButton(vec2(660, 240), vec3(1,1,0), 0.6f, "T-pose",4);
        btn5->RegistCallback(f1);
        lod1 = new engine::UIButton(vec2(740, 360), vec3(1,1,0), 0.6f, "lod1",0);
        lod1->RegistCallback(f2);
        lod2 = new engine::UIButton(vec2(740, 330), vec3(1,1,0), 0.6f, "lod2",2);
        lod2->RegistCallback(f2);
        lod3 = new engine::UIButton(vec2(740, 300), vec3(1,1,0), 0.6f, "lod3",3);
        lod3->RegistCallback(f2);
        lod4 = new engine::UIButton(vec2(740, 270), vec3(1,1,0), 0.6f, "lod4",4);
        lod4->RegistCallback(f2);
        lod5 = new engine::UIButton(vec2(740, 240), vec3(1,1,0), 0.6f, "lod5",5);
        lod5->RegistCallback(f2);
    }
    
    void InitScene()
    {
        shader = new LightShader("model.vs", "model.fs");
        halo = new Avatar("halo", vec3(-1.0f, -4.0f, -1.5f), vec3(1.0f), -60, shader);
    }

    void DrawScene()
    {
        halo->Rotate(0.2f);
        halo->Draw(shader, light, camera);
    }
    
    void Click(int anim)
    {
        switch (anim) {
            case 0:
                halo->PlayAnim("something");
                break;
            case 1:
                halo->PlayAnim("idle");
                break;
            case 2:
                halo->PauseAnim();
                break;
            case 3:
                halo->ResumeAnim();
                break;
            case 4:
                halo->SetTPose();
                break;
            default:
                break;
        }
    }
    
    void LodSelect(int lod)
    {
        halo->ChangeLOD(lod);
    }
    
    
private:
    LightShader* shader;
    Avatar *halo;
    engine::UIButton *btn1,*btn2,*btn3,*btn4,*btn5;
    engine::UIButton *lod1,*lod2,*lod3,*lod4,*lod5;
};


#endif /* scene5_h */
