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

    Avatar::Avatar(const char* name)
    {
        vector<string> items;
        ReadSummary(name, items);
        materials.clear();
        for (size_t i=0; i<items.size(); i++)
        {
            MeshData* data = ReadMesh(items[i]);
            Material* mat = new Material(data);
            materials.push_back(mat);
        }
    }


    Avatar::~Avatar()
    {
        for (size_t i=0;i<materials.size(); i++)
        {
            delete materials[i];
        }
        materials.clear();
    }


    mat4 Avatar::GetModelMatrix(vec3 pos, vec3 scale, float angle)
    {
        mat4 model(1);
        model = translate(model, pos);
        model = glm::scale(model, scale);
        model = rotate(model, radians(angle), vec3(0.0f,1.0f,0.0f));
        return model;
    }

    void Avatar::Draw(Shader* shader, Light* light, vec3 pos, vec3 scale, float angle)
    {
        shader->use();
        light->Apply(shader);
        shader->setMat4("model", GetModelMatrix(pos, scale, angle));
        LightShader* lshader =static_cast<LightShader*>(shader);
        if(lshader) lshader->ApplyLight();
        for (size_t i=0; i<materials.size(); i++)
        {
            materials[i]->Draw(shader);
        }
    }


    void Avatar::DrawShadow(Shader* shader, Light* light, vec3 pos, vec3 scale, float angle)
    {
        shader->use();
        shader->setMat4("model", GetModelMatrix(pos, scale, angle));
        for (size_t i=0; i<materials.size(); i++)
        {
            materials[i]->DrawMesh();
        }
    }


}
