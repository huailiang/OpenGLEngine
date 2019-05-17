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
  
    
    /*
     init: camera-> skybox-> light-> scene-> ui
     */
    void Initial()
    {
        if(camera == NULL)
        {
            camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
        }
        if(skybox == NULL)
        {
            skybox = new Skybox(camera);
        }
        if(light == NULL)
        {
            InitLight();
            InitScene();
        }
        DrawUI();
    }
    
    virtual int getType() = 0;
    
    virtual void InitLight() = 0;
    
    virtual void InitScene() { }
    
    virtual void DrawUI() { }
    
    virtual void DrawTerrain() = 0;
    
    virtual void DrawChar() { }
    
    void DrawScene()
    {
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawTerrain();
        if(skybox) skybox->Draw();
        DrawChar();
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

    
protected:
    Camera* camera;
    Light* light;
    Skybox* skybox;

};


#endif /* scene_h */
