//
//  scene3.h
//  LearnGL
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
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        delete shadowShader;
        delete depthShader;
        delete debugShader;
        depthShader = NULL;
        shadowShader = NULL;
        debugShader = NULL;
    }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,4.0f); }

    int getType() { return TY_Scene3; }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void InitScene()
    {
        depthShader  = new Shader("shader/shadowdepth.vs","shader/shadowdepth.fs");
        shadowShader  = new Shader("shader/shadow.vs","shader/shadow.fs");
        debugShader = new Shader("shader/debug.vs", "shader/debug.fs");
        
        InitPlane(planeVAO, planeVBO);
        InitCube(cubeVAO, cubeVBO);
        InitQuad(quadVAO, quadVBO);
        TTexture("resources/textures/wood.png", &woodTexture);
        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        shadowShader->use();
        shadowShader->setInt("diffuseTexture", 0);
        shadowShader->setInt("shadowMap", 1);
    }
    
    
    void DrawScene()
    {
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, near_plane, far_plane);
        lightView = static_cast<DirectLight*>(light)->GetLigthViewMatrix(glm::vec3(0,0,-2));
        lightSpaceMatrix = lightProjection * lightView;
        depthShader->use();
        depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        Scene::ClearScene();
        renderScene(*depthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        Scene::ClearScene();
        glViewport(0, 0, RENDER_WIDTH, RENDER_HEIGTH);
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
        renderScene(*shadowShader);
        renderQuad();
    }
    
    
    void renderScene(const Shader &shader)
    {
        // floor
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // cubes
        glBindVertexArray(cubeVAO);
        renderCube(glm::vec3(0.0f, 0.2f, -1.6), 0.5f, 0.0f, shader);
        renderCube(glm::vec3(2.0f, 0.0f, -1.0), 0.5f, 0.0f, shader);
        renderCube(glm::vec3(-1.0f,0.0f, -2.0), 0.25f, 16 * glfwGetTime(), shader);
        glBindVertexArray(0);
    }
    
    void renderQuad()
    {
        debugShader->use();
        debugShader->setFloat("near_plane", 1.0f);
        debugShader->setFloat("far_plane", 7.5f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    
    void renderCube(glm::vec3 pos, float scale, float ang, const Shader &shader)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(ang), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(scale));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    
private:
    Shader *depthShader, *shadowShader, *debugShader;
    unsigned int woodTexture ;
    unsigned int planeVBO, planeVAO;
    unsigned int cubeVAO, cubeVBO;
    unsigned int quadVAO, quadVBO;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    unsigned int depthMap;
};

#endif /* scene3_h */
