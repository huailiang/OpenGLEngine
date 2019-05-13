//
//  terrain.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef terrain_h
#define terrain_h

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "std/shader.h"
#include "std/texture.h"

class Terrain
{
public:
    Terrain()
    {
        shader=new Shader("shader/terrain.vs","shader/terrain.fs");
        TTexture texture("resources/textures/metal.png", &floorTexture);
        TTexture texture2("resources/textures/grass.png", &grassTexture,false,GL_CLAMP_TO_EDGE);
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
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            5.0f, -0.5f, -5.0f,  2.0f, 2.0f
        };

        float grass_verts[] = {
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
        };
        
        vegetation =
        {
            vec3(-2.5f, 0.0f, -2.48f),
            vec3( 2.5f, 0.0f, -1.51f),
            vec3( 0.0f, 0.0f, -1.7f),
            vec3(-1.3f, 0.0f, -2.3f),
            vec3 (1.5f, 0.0f, -0.6f)
        };

        glGenVertexArrays(1, &floor_vao);
        glGenBuffers(1, &floor_vbo);
        glBindVertexArray(floor_vao);
        glBindBuffer(GL_ARRAY_BUFFER,floor_vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(floor_verts), floor_verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE,5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
        
        glGenVertexArrays(1, &grass_vao);
        glGenBuffers(1, &grass_vbo);
        glBindVertexArray(grass_vao);
        glBindBuffer(GL_ARRAY_BUFFER,grass_vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(grass_verts), grass_verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE,5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
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
        
        glBindVertexArray(grass_vao);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        for (GLuint i=0; i<vegetation.size(); i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, vegetation[i]);
            glm::mat4 mvp2 = camera->GetVP() * model;
            shader->setMat4("mvp", mvp2);
            glDrawArrays(GL_TRIANGLES,0,6);
        }
        glBindVertexArray(0);
    }
    
private:
    unsigned int floor_vao, floor_vbo;
    unsigned int grass_vao, grass_vbo;
    unsigned int floorTexture, grassTexture;
    std::vector<vec3> vegetation;
    Shader* shader;
};

#endif /* terrain_h */
