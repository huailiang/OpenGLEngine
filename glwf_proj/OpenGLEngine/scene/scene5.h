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
        SAFE_DELETE(lod1);
        SAFE_DELETE(lod2);
        SAFE_DELETE(lod3);
        SAFE_DELETE(lod4);
    }
    
    int getType() { return TY_Scene5; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,16.0f); }
    
    static void OnClick(UIEvent* e, void* arg)
    {
        Scene5* scene = (Scene5*)(arg);
        int evtid = e->evtid;
        scene->Click(evtid);
    }
    
    static void OnLodClick(UIEvent* e, void* arg)
    {
        Scene5* scene = (Scene5*)(arg);
        int evtid = e->evtid;
        scene->LodSelect(evtid);
    }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        btn1 = new UIButton(vec2(660, 360), vec3(1,1,0), 0.6f, " pose ",0);
        btn1->RegistCallback(OnClick, this);
        btn2 = new UIButton(vec2(660, 330), vec3(1,1,0), 0.6f, " idle ",1);
        btn2->RegistCallback(OnClick, this);
        btn3 = new UIButton(vec2(660, 300), vec3(1,1,0), 0.6f, "pause",2);
        btn3->RegistCallback(OnClick, this);
        btn4 = new UIButton(vec2(660, 270), vec3(1,1,0), 0.6f, "resume",3);
        btn4->RegistCallback(OnClick, this);
        lod1 = new UIButton(vec2(740, 360), vec3(1,1,0), 0.6f, "lod1",0);
        lod1->RegistCallback(OnLodClick, this);
        lod2 = new UIButton(vec2(740, 330), vec3(1,1,0), 0.6f, "lod2",2);
        lod2->RegistCallback(OnLodClick, this);
        lod3 = new UIButton(vec2(740, 300), vec3(1,1,0), 0.6f, "lod3",3);
        lod3->RegistCallback(OnLodClick, this);
        lod4 = new UIButton(vec2(740, 270), vec3(1,1,0), 0.6f, "lod4",5);
        lod4->RegistCallback(OnLodClick, this);
    }
    
    void InitScene()
    {
        shader = new LightShader("model.vs", "model.fs", nullptr, Macro("VERT_TYPE","0x2111"));
        ApplyCamera(shader);
        halo = new Avatar("halo", vec3(-1.0f, -4.0f, -1.5f), vec3(1.0f), -60);
    }

    void DrawScene()
    {
        halo->Rotate(0.2f);
        halo->Draw(shader, light);
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
    UIButton *btn1,*btn2,*btn3,*btn4;
    UIButton *lod1,*lod2,*lod3,*lod4;
};


#endif /* scene4_h */
