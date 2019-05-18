//
//  scene2.h
//  LearnGL
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
        delete shader;
        shader = NULL;
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
    
    int getType()
    {
        return TY_Scene2;
    }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,0.0f,0.0f));
    }
    
    void InitScene()
    {
        InitCube(vao, vbo);
        shader = new LightShader("shader/light.vs","shader/light.fs");
        shader->use();
        shader->setInt("texture1", 0);
        shader->setInt("texture2", 1);
        shader->setVec3("viewPos", camera->Position);
        shader->ApplyLight();

        TTexture("resources/textures/container.jpg", &texture1);
        TTexture("resources/textures/awesomeface.png", &texture2);
    }

    
    void DrawUI()
    {
        Scene::DrawUI();
    }
    
    
    void DrawScene()
    {
        Scene::ClearScene();
        
        float timeValue = glfwGetTime();
        vec3 cubePositions[] = { glm::vec3( 0.0f,  0.0f,  -2.0f), glm::vec3( 2.0f,  1.0f, -4.0f) };
        mat4 view = camera->GetViewMatrix();
        mat4 proj = camera->GetProjMatrix();
        shader->use();
        shader->setFloat("scale", 1);
        shader->setMat4("view", view);
        shader->setMat4("projection", proj);
        light->Apply(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        glBindVertexArray(vao);
        for (unsigned int i = 0; i < 2; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = translate(model, cubePositions[i]);
            float angle = 64 * i * timeValue;
            model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, vec3(0.5f));
            shader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    
private:
    unsigned int vbo, vao;
    unsigned int texture1, texture2;
    LightShader* shader;
};


#endif /* scene2_h */
