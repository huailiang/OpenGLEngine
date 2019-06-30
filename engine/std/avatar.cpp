//
//  avatar.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "avatar.h"

namespace engine
{

    Avatar::Avatar(const char* name, glm::vec3 pos, glm::vec3 scale, float angle)
    {
        vector<string> items;
        this->name = name;
        this->pos = pos;
        this->scale = scale;
        this->angle= angle;
        RecalModelMatrix();
        type = ReadSummary(name, items);
        materials.clear();
        meshs.clear();
        for (size_t i=0; i<items.size(); i++)
        {
            MeshData* mesh = ReadMesh(items[i]);
            ObjMaterial* mat = new ObjMaterial(mesh);
            ReadObjMaterial(items[i], mat);
            mat->SetupMesh();
            materials.push_back(mat);
            meshs.push_back(mesh);
        }
        if (type & MODEL_ANI)
        {
            LoadSkeleton();
        }
    }


    Avatar::~Avatar()
    {
        loop(materials.size())  delete materials[i];
        loop(meshs.size()) delete meshs[i];
        materials.clear();
        SAFE_DELETE(skeleton);
    }


    void Avatar::RecalModelMatrix()
    {
        mat4 model(1);
        model = translate(model, pos);
        model = glm::scale(model, scale);
        model = rotate(model, radians(angle), vec3(0.0f,1.0f,0.0f));
        matrix = model;
    }
    

    void Avatar::Draw(Shader* shader, Light* light, Camera* camera)
    {
        loop(materials.size())  materials[i]->Draw(shader);//must put at first to bind mesh
        shader->use();
        light->Apply(shader);
        shader->setMat4("model", matrix);
        LightShader* lshader =static_cast<LightShader*>(shader);
        if(lshader) lshader->ApplyLight();
        if(skeleton) skeleton->Draw(shader);
        if(camera) camera->Attach(shader);
    }


    void Avatar::DrawShadow(Shader* shader, Light* light, Camera* camera)
    {
        loop(materials.size()) materials[i]->DrawMesh();
        shader->use();
        shader->setMat4("model", matrix);
        if(camera) camera->Attach(shader);
    }
    
    
    void Avatar::Rotate(float delta)
    {
        angle += delta;
        RecalModelMatrix();
    }
    
    
    void Avatar::Move(glm::vec3 move)
    {
        this->pos += move;
        RecalModelMatrix();
    }
    
    
    void Avatar::Scale(float scale)
    {
        this->scale *= scale;
        RecalModelMatrix();
    }
    
    
    void Avatar::Scale(glm::vec3 scale)
    {
        this->scale = scale;
        RecalModelMatrix();
    }
    
    
    void Avatar::LoadSkeleton()
    {
        if(skeleton == nullptr)
        {
            skeleton = new Skeleton();
        }
        ReadSkeleton(skeleton, name, name);
        skeleton->PlayAnim("something");
    }
    
    
    void Avatar::PlayAnim(std::string anim)
    {
        if(skeleton)
        {
            skeleton->PlayAnim(anim);
        }
    }
    
    
    void Avatar::PauseAnim()
    {
        if(skeleton)
        {
            skeleton->Pause();
        }
    }
    
    void Avatar::ResumeAnim()
    {
        if(skeleton)
        {
            skeleton->Resume();
        }
    }
    
    void Avatar::SetTPose()
    {
        if(skeleton)
        {
            skeleton->SetTPose();
        }
    }
    
    void Avatar::ChangeLOD(short ilod)
    {
        for (size_t i=0; i<materials.size(); i++) {
            MeshData* data = materials[i]->mesh;
            RecalcuteLod(data, "halo", "halo", ilod);
            materials[i]->SetupMesh();
        }
    }
    
}
