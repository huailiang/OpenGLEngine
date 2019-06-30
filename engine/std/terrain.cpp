//
//  terrain.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/29/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "terrain.h"
#include "asset.h"

namespace engine
{

    Terrain::Terrain()
    {
        shader = new Shader("sample.vs","sample.fs");
        shader2 = new Shader("instance.vs","instance.fs");
        
        Texture("textures/metal", PNG, &floorTexture);
        Texture("textures/grass",PNG, &grassTexture,false,GL_CLAMP_TO_EDGE);
        initial();
    }

    Terrain::~Terrain()
    {
        SAFE_DELETE(shader);
        SAFE_DELETE(shader2);
        SAFE_DELETE(gdata);
        glDeleteBuffers(1, &floor_vbo);
        glDeleteVertexArrays(1,&floor_vbo);
    }

    void Terrain::initial()
    {        
        grass_num = 800;
        for(int i=0; i<grass_num; i++)
        {
            float x = (rand() % 300)/10.0f - 15.0f;
            float z = (rand() % 300)/10.0f - 15.0f;
            vegetation.push_back(vec3(x,0.0f,z));
        }
        
        InitPlane(&floor_vao, &floor_vbo, shader);
        gdata=ReadMesh("grass","common");
        glGenVertexArrays(1, &grass_vao);
        glGenBuffers(1, &grass_vbo);
        glGenBuffers(1, &instVbo);
        glBindVertexArray(grass_vao);
        glBindBuffer(GL_ARRAY_BUFFER,grass_vbo);
        gdata->ConfigAttribute();
    #ifdef _Instance_
        InitInstance();
    #endif
    }

    void Terrain::InitInstance()
    {
        glm::mat4* modelMatrices;
        modelMatrices = new glm::mat4[grass_num];
        for(int i=0;i<grass_num;i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, vegetation[i]);
            modelMatrices[i] = model;
        }
        glBindBuffer(GL_ARRAY_BUFFER, instVbo);
        glBufferData(GL_ARRAY_BUFFER, grass_num * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        shader->use();
        shader->setInt("texture1",  0);
        shader->setInt("shadow", 1);
        shader2->use();
        shader2->setInt("texture1", 0);
    }


    void Terrain::DrawShadow(const Shader *shader)
    {
        shader->setMat4("model", glm::mat4(1));
        glBindVertexArray(floor_vao);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
    }

    void Terrain::Draw(Camera* camera, glm::mat4 lightMatrix, Light* light, GLuint depthMap)
    {
        shader->use();
        glBindVertexArray(floor_vao);
        shader->setMat4("vp", camera->GetVP());
        shader->setMat4("model", glm::mat4(1));
        light->Apply(shader);
        shader->setMat4("lightSpaceMatrix", lightMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDrawArrays(DRAW_MODE,0,6);
        glBindVertexArray(0);
        
    #ifdef _Instance_
        DrawGrassInstance(camera);
    #else
        DrawGrassDirect(camera);
    #endif
    }

    void Terrain::DrawSample(Camera* camera)
    {
        shader->use();
        glBindVertexArray(floor_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader->setMat4("vp", camera->GetVP());
        shader->setMat4("model", glm::mat4(1));
        glDrawArrays(DRAW_MODE,0,6);
        glBindVertexArray(0);
        
    #ifdef _Instance_
        DrawGrassInstance(camera);
    #else
        DrawGrassDirect(camera);
    #endif
    }

    void Terrain::DrawGrassDirect(Camera* camera)
    {
        shader2->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glBindVertexArray(grass_vao);
        for (GLuint i=0; i<grass_num; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, vegetation[i]);
            glm::mat4 mvp = camera->GetVP() * model;
            shader2->setMat4("mvp", mvp);
            glDrawArrays(DRAW_MODE,0,6);
        }
        glBindVertexArray(0);
    }

    void Terrain::DrawGrassInstance(Camera* camera)
    {
        shader2->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glm::mat4 model(1.0f);
        shader2->setMat4("mvp", camera->GetVP() * model);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glBindVertexArray(grass_vao);
        glDrawArraysInstanced(DRAW_MODE, 0, 6, grass_num);
        glBindVertexArray(0);
    }


}
