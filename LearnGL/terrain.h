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
        initial();
    }
    
    ~Terrain()
    {
        delete shader;
    }
    
    void initial()
    {
        float vertices[] = {
            // positions         texture Coords
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            5.0f, -0.5f, -5.0f,  2.0f, 2.0f
        };

        
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE,5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }
    
    void Draw(glm::mat4 mvp)
    {
        shader->use();
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader->setMat4("mvp", mvp);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
    }
    
private:
    unsigned int vao, vbo;
    unsigned int floorTexture;
    Shader* shader;
};

#endif /* terrain_h */
