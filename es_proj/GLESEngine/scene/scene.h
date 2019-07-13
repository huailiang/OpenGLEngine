//
//  scene1.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include "engine.h"
#include "camera.h"
#include "light.h"
#include "uimgr.h"
#include "label.h"
#include "button.h"
#include "uievent.h"
#include "shader.h"
#include "texture.h"
#include "skybox.h"

#define TY_Scene1 0
#define TY_Scene2 1
#define TY_Scene3 2
#define TY_Scene4 3
#define TY_Scene5 4

using namespace engine;

class Scene
{
    
public:
    
    virtual ~Scene()
    {
        SAFE_DELETE(camera);
        SAFE_DELETE(skybox);
        SAFE_DELETE(light);
        SAFE_DELETE(depthShader);
        SAFE_DELETE(debugShader);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteFramebuffers(1, &depthMapFBO);
        glCheckError();
    }
    
    virtual bool drawShadow()
    {
#ifdef _GLES_
        return false;
#else
        return true;
#endif
    }
  
    virtual glm::vec3 getCameraPos()
    {
        return glm::vec3(0.0f,0.0f,3.0f);
    }
    
    virtual std::string getSkybox()
    {
        return "lake";
    }
    
    virtual bool isEquirectangularMap() { return false; }
    
    /*
     init: camera-> skybox-> light-> scene-> ui
     */
    void Initial()
    {
        glCheckError();
        if(drawShadow())
        {
            depthShader  = new Shader("depth.vs","depth.fs");
            debugShader = new Shader("debug.vs", "debug.fs");
            debugShader->attach("_DEBUG_DEPTH_");
            InitDepthBuffer();
            glCheckError();
            InitQuad(&quadVAO, &quadVBO, debugShader);
        }
        camera = new Camera(getCameraPos());
        skybox = new Skybox(camera, getSkybox(), isEquirectangularMap());
        lightMatrix = glm::mat4(1);
        InitLight();
        InitScene();
        DrawUI();
    }
    
    virtual int getType() = 0;
    
    virtual void InitLight() = 0;
    
    virtual void InitScene() { }
    
    virtual void DrawUI() { }
    
    virtual void DrawShadow(Shader *depthShader)
    {
        float near_plane = 0.1f, far_plane = 7.5f;
        if(light->getType() == LightDirect)
        {
            lightMatrix = static_cast<DirectLight*>(light)->GetLigthSpaceMatrix(glm::vec3(0,0,-2),near_plane, far_plane, 4, 4);
        }
        else
        {
            lightMatrix = dynamic_cast<PointLight*>(light)->GetLigthSpaceMatrix(near_plane, far_plane, 4, 4);
        }
        depthShader->use();
        depthShader->setMat4("lightSpaceMatrix", lightMatrix);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    }
    
    virtual void DrawScene() { }
    
    virtual void OnLightChange(int key) { }
    
    void ClearScene()
    {
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0x00);
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    
    void DrawScenes()
    {
        timeValue = GetRuntime();
        if(drawShadow())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            ClearScene();
            DrawShadow(depthShader);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        ClearScene();
        glViewport(0, 0, RENDER_WIDTH, RENDER_HEIGTH);
        DrawScene();
        if(drawShadow() && debug) RenderQuad();
        if(skybox) skybox->Draw();
    }
    
    void RebuildSky()
    {
        delete skybox;
        skybox = new Skybox(camera, getSkybox(), isEquirectangularMap());
    }
    
    void ProcessMouseMove(double xoffset,double yoffset)
    {
        if(camera)
        {
            camera->ProcessMouseMovement(xoffset, yoffset);
        }
    }
    
    void ProcessMouseScroll(double xoffset, double yoffset)
    {
        if(camera)
        {
            camera->ProcessMouseScroll(yoffset);
        }
    }
    
    
protected:
    
    void ApplyCamera(Material* mat)
    {
        if(mat && mat->shader)
        {
            camera->Attach(mat->shader);
        }
    }
    
    void ApplyCamera(Shader* shader)
    {
        camera->Attach(shader);
    }

    void RenderQuad(GLuint map)
    {
        debugShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, map);
        DrawQuad();
    }
        
private:
    void InitDepthBuffer()
    {
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glGenFramebuffers(1, &depthMapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void RenderQuad()
    {
        debugShader->use();
        debugShader->setFloat("near_plane", 1.0f);
        debugShader->setFloat("far_plane", 7.5f);
        RenderQuad(depthMap);
    }
    
    void DrawQuad()
    {
        glBindVertexArray(quadVAO);
        glDrawArrays(DRAW_MODE, 0, 6);
        glBindVertexArray(0);
    }

    
protected:
    Camera* camera = nullptr;
    Light*  light = nullptr;
    Skybox* skybox = nullptr;
    uint SHADOW_WIDTH = 256, SHADOW_HEIGHT = 256;
    GLuint depthMap;
    float timeValue;
    mat4 lightMatrix;
    bool debug;
    GLuint quadVAO, quadVBO;
    Shader* depthShader = nullptr;
    Shader* debugShader = nullptr;
    GLuint depthMapFBO;
};


#endif /* scene_h */
