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
        for (size_t i=0; i<items.size(); i++)
        {
            ObjMaterial* mat = new ObjMaterial(ReadMesh(items[i]));
            ReadObjMaterial(items[i], mat);
            mat->SetupMesh();
            materials.push_back(mat);
        }
        if (type & MODEL_ANI)
        {
            LoadSkeleton();
        }
    }


    Avatar::~Avatar()
    {
        loop(materials.size())  delete materials[i];
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
    

    void Avatar::Draw(Shader* shader, Light* light)
    {
        shader->use();
        light->Apply(shader);
        shader->setMat4("model", matrix);
        LightShader* lshader =static_cast<LightShader*>(shader);
        if(lshader) lshader->ApplyLight();
        for (size_t i=0; i<materials.size(); i++)
        {
            materials[i]->Draw(shader);
        }
        if(skeleton)
        {
            skeleton->Draw(shader);
        }
    }


    void Avatar::DrawShadow(const Shader* shader, Light* light)
    {
        shader->use();
        shader->setMat4("model", matrix);
        for (size_t i=0; i<materials.size(); i++)
        {
            materials[i]->DrawMesh();
        }
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
//        skeleton->PlayAnim("something");
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
    
    void Avatar::ChangeLOD(short ilod)
    {
        for (size_t i=0; i<materials.size(); i++) {
            MeshData* data = materials[i]->data;
            RecalcuteLod(data, "halo", "halo", ilod);
            materials[i]->SetupMesh();
        }
    }
    
}
