//
//  scene2.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene2_h
#define scene2_h

#include "scene.h"

class Scene2 : public Scene
{
    
public:
    
    ~Scene2()
    {
        SAFE_DELETE(mat);
        SAFE_DELETE(shader);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        SAFE_DELETE(btn_direct);
        SAFE_DELETE(btn_point);
        SAFE_DELETE(btn_spot);
    }
    
    std::string getSkybox() { return "mp_5dim"; }
    
    int getType() { return TY_Scene2; }
    
    bool drawShadow() { return false; }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(-1,0,-7));
    }
    
    void InitScene()
    {
        MeshData* cube =(MeshData*)InitCube(&vao, &vbo);
        mat= new Material(cube);
        InitShader();
    }
    
    void InitShader()
    {
        shader = new LightShader("light.vs","light.fs");
        shader->attach(light->getMacro().c_str());
        mat->AttachShader(shader);
        shader->use();
        mat->SetFloat("scale", 1);
        mat->SetTexture("texture1", "textures/container", JPG);
        mat->SetTexture("texture2", "textures/awesomeface", PNG);
        ApplyCamera(mat);
    }

    
    void DrawUI()
    {
        Scene::DrawUI();
        btn_direct = new UIButton(vec2(720, 360), vec3(1,1,0), 0.6f, " dirct-light", 0);
        btn_direct->RegistCallback(OnLightClick, this);
        btn_point = new UIButton(vec2(720, 330), vec3(1,1,0), 0.6f, "point-light", 1);
        btn_point->RegistCallback(OnLightClick, this);
        btn_spot = new UIButton(vec2(720, 300), vec3(1,1,0), 0.6f, " spot-light", 2);
        btn_spot->RegistCallback(OnLightClick, this);
    }
    
    
    void DrawScene()
    {
        Scene::ClearScene();
        vec3 cubePositions[] = { glm::vec3( 0.0f,  0.0f,  -2.0f), glm::vec3(2.0f,  1.0f, -4.0f) };
        mat->Draw();
        light->Apply(mat);
        glBindVertexArray(vao);
        for (uint i = 0; i < 2; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = translate(model, cubePositions[i]);
            float angle = 64 * i * timeValue;
            model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, vec3(0.5f));
            mat->SetMat4("model", model);
            glDrawArrays(DRAW_MODE, 0, 36);
        }
    }
    
    void OnLightChange(int key)
    {
        if(light && mat)
        {
            light->Apply(mat);
        }
    }
    
    void HandleClick(int evtid)
    {
        delete shader;
        delete light;
        if(evtid == 0) light = new DirectLight(vec3(1), vec3(-1,0,-2));
        if(evtid == 1) light = new PointLight(vec3(1), vec3(0,0,-1), vec3(0,0,2), vec3(0.1,0.2,0.01));
        if(evtid == 2) light = new SpotLight(vec3(1), vec3(0,0,-1), vec3(0,0,2), vec3(1,0.1,0), 6, 9);
        InitShader();
    }
    
private:
    static void OnLightClick(UIEvent* contex, void* arg)
    {
        int evtid = contex->evtid;
        Scene2* scene = (Scene2*)(arg);
        scene->HandleClick(evtid);
    }

    
private:
    UIButton* btn_direct, *btn_point, *btn_spot;
    Material* mat = nullptr;
    GLuint vbo, vao;
    LightShader* shader;
};


#endif /* scene2_h */
