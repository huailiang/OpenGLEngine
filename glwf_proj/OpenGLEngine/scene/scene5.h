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
    }
    
    int getType() { return TY_Scene5; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,16.0f); }
    
    static void OnClick(UIEvent* e, void* arg)
    {
        Scene5* scene = (Scene5*)(arg);
        int evtid = e->evtid;
        scene->Click(evtid);
    }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        btn1 = new UIButton(vec2(720, 360), vec3(1,1,0), 0.6f, " pose ",0);
        btn1->RegistCallback(OnClick, this);
        btn2 = new UIButton(vec2(720, 330), vec3(1,1,0), 0.6f, " idle ",1);
        btn2->RegistCallback(OnClick, this);
        btn2 = new UIButton(vec2(720, 300), vec3(1,1,0), 0.6f, "pause",2);
        btn2->RegistCallback(OnClick, this);
        btn2 = new UIButton(vec2(720, 270), vec3(1,1,0), 0.6f, "resume",3);
        btn2->RegistCallback(OnClick, this);
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
    
    
private:
    LightShader* shader;
    Avatar *halo;
    UIButton *btn1,*btn2,*btn3,*btn4;
};


#endif /* scene4_h */
