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
#include "camera.hpp"
#include "light.hpp"
#include "uimgr.hpp"
#include "label.hpp"
#include "button.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "skybox.hpp"
#include "iScene.h"

#define TY_Scene1 0
#define TY_Scene2 1
#define TY_Scene3 2
#define TY_Scene4 3
#define TY_Scene5 4
#define TY_Scene6 5
#define TY_Scene7 6

using namespace engine;

class Scene : iScene
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
    
    Camera* getCamera() { return camera; }
    
    /*
     * init seq: camera-> skybox-> light-> scene-> ui
     */
    void Initial()
    {
        glCheckError();
        debugShader = new Shader("debug.vs", "debug.fs");
        if(drawShadow())
            debugShader->attach("_DEBUG_DEPTH_");
        if(isARScene())
            debugShader->attach("_FLIP_Y_");
        InitQuad(&quadVAO, &quadVBO, debugShader);
        if(drawShadow())
        {
            depthShader  = new Shader("depth.vs","depth.fs");
            InitDepthBuffer();
            glCheckError();
        }
        camera = new Camera(getCameraPos());
        string sky = getSkybox();
        if(!sky.empty()) skybox = new Skybox(camera, sky, isEquirectangularMap());
        lightMatrix = glm::mat4(1);
        InitLight();
        InitScene();
        DrawUI();
    }
    
    virtual int getType() = 0;
    
    virtual void InitLight() = 0;
    
    virtual void InitScene() { }
    
    virtual void DrawUI() { }
    
    virtual bool isARScene() { return false; }
    
    virtual void DrawShadow(Shader *depthShader)
    {
        float near_plane = 0.1f, far_plane = 7.5f;
        lightMatrix = light->GetLigthSpaceMatrix(near_plane, far_plane, 4, 4);
        depthShader->use();
        depthShader->setMat4("lightSpaceMatrix", lightMatrix);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    }
    
    virtual void DrawScene() { }
    
    virtual void OnLightChange(int key) { }
    
    void ClearScene()
    {
        glDepthMask(true);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        glCheckError();
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
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
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
