//
//  scene3.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene3_h
#define scene3_h

#include "scene.h"

class Scene3 : public Scene
{
    
public:
    ~Scene3()
    {
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &planeVBO);
        glDeleteBuffers(1, &cubeVBO);
        delete shadowShader;
        shadowShader = NULL;
    }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,4.0f); }

    int getType() { return TY_Scene3; }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void InitScene()
    {
        debug = true;
        shadowShader  = new Shader("shadow.vs","shadow.fs");
        InitPlane(planeVAO, planeVBO);
        InitCube(cubeVAO, cubeVBO);
        
        TTexture("resources/textures/wood.png", &woodTexture);
        shadowShader->use();
        shadowShader->setInt("diffuseTexture", 0);
        shadowShader->setInt("shadowMap", 1);
    }
    
    void DrawShadow(Shader *depthShader)
    {
        Scene::DrawShadow(depthShader);
        RenderScene(depthShader);
    }

    
    void DrawScene()
    {
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 7.5f;
        lightSpaceMatrix = static_cast<DirectLight*>(light)->GetLigthSpaceMatrix(glm::vec3(0,0,-2), near_plane, far_plane, 4, 4);
        shadowShader->use();
        shadowShader->setMat4("projection", camera->GetProjMatrix());
        shadowShader->setMat4("view", camera->GetViewMatrix());
        shadowShader->setVec3("viewPos", camera->Position);
        light->Apply(shadowShader);
        shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        RenderScene(shadowShader);
    }
    
    
    void RenderScene(const Shader *shader)
    {
        // floor
        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // cubes
        renderCube(glm::vec3(0.0f, 0.2f, -1.6), 0.5f, 0.0f, shader);
        renderCube(glm::vec3(2.0f, 0.0f, -1.0), 0.5f, 0.0f, shader);
        renderCube(glm::vec3(-1.0f,0.0f, -2.0), 0.25f, 16 * timeValue, shader);
    }
    
    
    void renderCube(glm::vec3 pos, float scale, float ang, const Shader *shader)
    {
        glBindVertexArray(cubeVAO);
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(ang), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(scale));
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    
private:
    Shader *shadowShader;
    unsigned int woodTexture ;
    unsigned int planeVBO, planeVAO;
    unsigned int cubeVAO, cubeVBO;
    
};

#endif /* scene3_h */
