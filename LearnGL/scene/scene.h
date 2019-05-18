//
//  scene1.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include "../common.h"
#include "../std/camera.h"
#include "../std/light.h"
#include "../gui/uimgr.h"
#include "../gui/label.h"
#include "../gui/uievent.h"
#include "../std/shader.h"
#include "../std/texture.h"


#define TY_Scene1 0
#define TY_Scene2 1
#define TY_Scene3 2


class Scene
{
    
public:
    
    virtual ~Scene()
    {
        delete camera;
        delete skybox;
        delete light;
        camera = NULL;
        light = NULL;
        skybox = NULL;
    }
    
    virtual bool drawShadow()
    {
        return true;
    }
  
    virtual glm::vec3 getCameraPos()
    {
        return glm::vec3(0.0f,0.0f,3.0f);
    }
    
    /*
     init: camera-> skybox-> light-> scene-> ui
     */
    void Initial()
    {
        camera = new Camera(getCameraPos());
        skybox = new Skybox(camera);
        InitLight();
        InitScene();
        if(drawShadow())
        {
            depthShader  = new Shader("shader/depth.vs","shader/depth.fs");
            InitDepthBuffer();
        }
        DrawUI();
    }
    
    virtual int getType() = 0;
    
    virtual void InitLight() = 0;
    
    virtual void InitScene() { }
    
    virtual void DrawUI() { }
    
    virtual void DrawShadow(Shader *depthShader)
    {
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 7.5f;
        lightSpaceMatrix = static_cast<DirectLight*>(light)->GetLigthSpaceMatrix(glm::vec3(0,0,-2), near_plane, far_plane, 4, 4);
        depthShader->use();
        depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    }
    
    virtual void DrawScene() { }
    
    void ClearScene()
    {
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void DrawScenes()
    {
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
        if(skybox) skybox->Draw();
    }
    
    void ProcessKeyboard(GLFWwindow *window, float deltatime)
    {
        if(camera)
        {
            if (glfwGetKey(window, GLFW_KEY_A)== GLFW_PRESS)
                camera->ProcessKeyboard(LEFT, deltatime);
            if (glfwGetKey(window, GLFW_KEY_D)== GLFW_PRESS)
                camera->ProcessKeyboard(RIGHT, deltatime);
            if (glfwGetKey(window, GLFW_KEY_W)== GLFW_PRESS)
                camera->ProcessKeyboard(FORWARD, deltatime);
            if (glfwGetKey(window, GLFW_KEY_S)== GLFW_PRESS)
                camera->ProcessKeyboard(BACKWARD, deltatime);
            if (glfwGetKey(window, GLFW_KEY_SPACE)== GLFW_PRESS)
            {
                float timeValue = glfwGetTime()*0.04f;
                float ang = radians(timeValue);
                vec3 center = vec3(0.0f, 0.0f, -2.0f);
                vec3 pos = camera->Position;
                vec3 npos = pos;
                npos.x = (pos.x - center.x) * cos(ang) - (pos.z- center.z)*sin(ang) + center.x;
                npos.z = (pos.z - center.z) * cos(ang) + (pos.x - center.x) * sin(ang) + center.z;
                camera->RotateAt(npos, center);
            }
        }
        if(light)
        {
            if ( glfwGetKey(window, GLFW_KEY_LEFT)== GLFW_PRESS)
                light->UpdateX(-0.5f * deltatime);
            if ( glfwGetKey(window, GLFW_KEY_RIGHT)== GLFW_PRESS)
                light->UpdateX(0.5f * deltatime);
            if ( glfwGetKey(window, GLFW_KEY_UP)== GLFW_PRESS)
                light->UpdateY(0.5f * deltatime);
            if ( glfwGetKey(window, GLFW_KEY_DOWN)== GLFW_PRESS)
                light->UpdateY(-0.5f * deltatime);
        }
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

private:
    void InitDepthBuffer()
    {
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
    }
    
protected:
    Camera* camera;
    Light*  light;
    Skybox* skybox;
    Shader* depthShader;
    unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMap;
    
private:
    unsigned int depthMapFBO;
};


#endif /* scene_h */
