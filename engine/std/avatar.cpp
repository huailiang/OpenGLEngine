//
//  avatar.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "avatar.hpp"

namespace engine
{

    Avatar::Avatar(const char* name, glm::vec3 pos, glm::vec3 scale, float angle, Shader* shader) :
    Transform(pos, angle, scale)
    {
        vector<string> items;
        this->name = name;
        type = ReadSummary(name, items);
        materials.clear();
        meshs.clear();
        loop(items.size())
        {
            MeshData* mesh = ReadMesh(items[i]);
            ObjMaterial* mat = new ObjMaterial(mesh, shader);
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
    
    void Avatar::BindVert(Shader* shader)
    {
        loop(materials.size()) materials[i]->BindVert(shader);
    }

    void Avatar::Compile(Shader* shader)
    {
        loop(materials.size()) materials[i]->Compile(shader);
    }
    
    void Avatar::Draw(Shader* shader, Light* light, Camera* camera)
    {
        loop(materials.size())  materials[i]->Draw(shader);
        shader->use();
        light->Apply(shader);
        shader->setMat4("model", getWorldMatrix());
        LightShader* lshader =static_cast<LightShader*>(shader);
        if(lshader) lshader->ApplyLight();
        if(skeleton) skeleton->Draw(shader);
        if(camera) camera->Attach(shader);
    }


    void Avatar::DrawShadow(Shader* shader, Light* light, Camera* camera)
    {
        loop(materials.size()) materials[i]->DrawMesh();
        shader->use();
        shader->setMat4("model", getWorldMatrix());
        if(camera) camera->Attach(shader);
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
        loop (materials.size()) {
            MeshData* data = materials[i]->mesh;
            RecalcuteLod(data, "halo", "halo", ilod);
            materials[i]->SetupMesh();
        }
    }
    
}
