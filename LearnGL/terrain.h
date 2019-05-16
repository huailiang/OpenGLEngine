//
//  terrain.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef terrain_h
#define terrain_h

#include "common.h"
#include "std/shader.h"
#include "std/texture.h"

#define _Instance_

class Terrain
{
public:
    Terrain()
    {
        shader=new Shader("shader/terrain.vs","shader/terrain.fs");
        shader2=new Shader("shader/grass.vs","shader/terrain.fs");
        TTexture("resources/textures/metal.png", &floorTexture);
        TTexture("resources/textures/grass.png", &grassTexture,false,GL_CLAMP_TO_EDGE);
        initial();
    }
    
    ~Terrain()
    {
        delete shader;
        glDeleteBuffers(1, &floor_vbo);
        glDeleteVertexArrays(1,&floor_vbo);
    }
    
    void initial()
    {
        // floor
        float floor_verts[] = {
            // positions         texture Coords
            15.0f, -0.5f,  15.0f,  2.0f, 0.0f,
            -15.0f, -0.5f,  15.0f,  0.0f, 0.0f,
            -15.0f, -0.5f, -15.0f,  0.0f, 2.0f,
            
            15.0f, -0.5f,  15.0f,  2.0f, 0.0f,
            -15.0f, -0.5f, -15.0f,  0.0f, 2.0f,
            15.0f, -0.5f, -15.0f,  2.0f, 2.0f
        };

        float grass_verts[] = {
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
        };
        
        grass_num=800;
        for(int i=0;i<grass_num;i++)
        {
            float x = (rand()%300)/10.0f-15.0f;
            float z = (rand()%300)/10.0f-15.0f;
            vegetation.push_back(vec3(x,0.0f,z));
        }
        
        glGenVertexArrays(1, &floor_vao);
        glGenBuffers(1, &floor_vbo);
        glBindVertexArray(floor_vao);
        glBindBuffer(GL_ARRAY_BUFFER,floor_vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(floor_verts), floor_verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
        
        glGenVertexArrays(1, &grass_vao);
        glGenBuffers(1, &grass_vbo);
        glGenBuffers(1, &instVbo);
        glBindVertexArray(grass_vao);
        glBindBuffer(GL_ARRAY_BUFFER,grass_vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(grass_verts), grass_verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE,5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        
#ifdef _Instance_
        InitInstance();
#endif
    }
    
    void InitInstance()
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
    }

    void Draw(Camera* camera)
    {
        glStencilMask(0x00);
        shader->use();
        glBindVertexArray(floor_vao);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glm::mat4 mvp = camera->GetVP() * glm::mat4(1);
        shader->setMat4("mvp", mvp);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
        
#ifdef _Instance_
        DrawGrassInstance(camera);
#else
        DrawGrassDirect(camera);
#endif
    }
    
    void DrawGrassDirect(Camera* camera)
    {
        shader2->use();
        glBindVertexArray(grass_vao);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        for (GLuint i=0; i<grass_num; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, vegetation[i]);
            glm::mat4 mvp = camera->GetVP() * model;
            shader2->setMat4("mvp", mvp);
            glDrawArrays(GL_TRIANGLES,0,6);
        }
        glBindVertexArray(0);
    }
    
    void DrawGrassInstance(Camera* camera)
    {
        shader2->use();
        glm::mat4 model(1.0f);
        shader2->setMat4("mvp", camera->GetVP() * model);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glBindVertexArray(grass_vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, grass_num);
        glBindVertexArray(0);
    }
    
    
private:
    unsigned int floor_vao, floor_vbo;
    unsigned int grass_vao, grass_vbo, instVbo;
    unsigned int floorTexture, grassTexture;
    std::vector<vec3> vegetation;
    unsigned int grass_num;
    Shader* shader;
    Shader* shader2;
};

#endif /* terrain_h */
