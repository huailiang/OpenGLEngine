//
//  scene7.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 7/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene7_h
#define scene7_h

#include "scene.h"
#include "avatar.h"


class Scene7 : public Scene
{

public:
    
    Scene7():Scene()
    {
        envs[0] = "LancellottiChapel";
        envs[1] = "hornstullsstrand";
        envs[2] = "Footballfield2";
        envs[3] = "SanFrancisco";
        envs[4] = "GoldenGateBridge3";
        env = envs[0];
    }

    ~Scene7()
    {
        SAFE_DELETE(shader);
        SAFE_DELETE(dragon);
        SAFE_DELETE(btn1);
        SAFE_DELETE(btn2);
        SAFE_DELETE(btn3);
        SAFE_DELETE(btn4);
        SAFE_DELETE(btn5);
    }

    int getType() { return TY_Scene7; }
    
    std::string getSkybox() { return env; }

    virtual bool drawShadow() { return false; }

    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void InitScene()
    {
        shader = new LightShader("ibl/harmonics.vs", "ibl/harmonics.fs");
        dragon = new Avatar("halo",vec3(0,-1,0), vec3(0.2f), -90);
        dragon->BindVert(shader);
        dragon->Compile();
        LoadCoef();
    }
    
    void LoadCoef()
    {
        string sh_coef_file = WORKDIR + string("tools/data/"+env+"/coefficients.txt");
        ifstream ifs(sh_coef_file);
        if (!ifs) throw runtime_error("open " + sh_coef_file + " failed");
        int i = 0;
        float r, g, b;
        while (ifs >> r >> g >> b)
        {
            coefs[i] = glm::vec3(r, g, b);
            i++;
        }
        shader->use();
        shader->setVec3("coef", 16, coefs[0]);
    }
    
    void DrawScene()
    {
        dragon->Draw(shader, light, camera);
        dragon->Rotate(deltatime*30);
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        btn1 = new UIButton(vec2(690, 360), vec3(1,1,0), 0.6f, envs[0],0);
        btn1->RegistCallback(OnClick, this);
        btn2 = new UIButton(vec2(690, 330), vec3(1,1,0), 0.6f, envs[1],1);
        btn2->RegistCallback(OnClick, this);
        btn3 = new UIButton(vec2(690, 300), vec3(1,1,0), 0.6f, envs[2],2);
        btn3->RegistCallback(OnClick, this);
        btn4 = new UIButton(vec2(690, 270), vec3(1,1,0), 0.6f, envs[3],3);
        btn4->RegistCallback(OnClick, this);
        btn5 = new UIButton(vec2(690, 240), vec3(1,1,0), 0.6f, envs[4],4);
        btn5->RegistCallback(OnClick, this);
    }
    
    static void OnClick(UIEvent* e, void* arg)
    {
        Scene7* scene = (Scene7*)(arg);
        int evtid = e->evtid;
        scene->Click(evtid);
    }

    void Click(int eid)
    {
        env= envs[eid];
        LoadCoef();
        RebuildSky();
    }
    
private:
    Shader *shader = nullptr;
    Avatar *dragon = nullptr;
    glm::vec3 coefs[16];
    std::string envs[5];
    std::string env;
    UIButton *btn1,*btn2,*btn3,*btn4,*btn5;
};

#endif /* scene7_h */
