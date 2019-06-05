//
//  main.cpp
//  Created by 彭怀亮 on 5/8/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//


#include "common.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "model.h"
#include "skybox.h"
#include "light.h"
#include "uimgr.h"
#include "eventmgr.h"
#include "label.h"
#include "profile.h"
#include "terrain.h"
#include "scene/scenemgr.h"
#include "tool.h"

using namespace std;
using namespace glm;

float deltatime,lastTime;

int run_engine();
void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

UIManager UIManager::instance;
EventMgr EventMgr::instance;
TTFont TTFont::instance;
SceneMgr SceneMgr::instance;



int main(int argc, const char * argv[])
{
    if (argc<=1)
    {
//        return tool::LoadObj("planet");
        return run_engine();
    }
    else
    {
        if(strcmp(argv[1], "tools") == 0)
        {
            string name(argv[2]);
            tool::LoadObj(name);
        }
        else
        {
            return run_engine();
        }
    }
    return 1;
}

int run_engine()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLEngine", NULL, NULL);
    if (window == NULL)
    {
        cout<<"failed created window"<<endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    TTFont::getInstance()->initial();
    SceneMgr::getInstance()->Init();
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        float timeValue = glfwGetTime();
        deltatime  = timeValue-lastTime;
        lastTime= timeValue;
        SceneMgr::getInstance()->Draw(deltatime);
        UIManager::getInstance()->Draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    SceneMgr::getInstance()->ProcessKeyboard(window, deltatime);
    if (glfwGetKey(window, GLFW_KEY_1)== GLFW_PRESS) SetRenderMode(GL_TRIANGLES);
    if (glfwGetKey(window, GLFW_KEY_2)== GLFW_PRESS) SetRenderMode(GL_LINES);
    if (glfwGetKey(window, GLFW_KEY_3)== GLFW_PRESS) SetRenderMode(GL_POINTS);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    RENDER_WIDTH = width;
    RENDER_HEIGTH = height;
    glViewport(0, 0, width, height);
}

float lastX = 400, lastY = 300;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    SceneMgr::getInstance()->ProcessMouseMove(xoffset, yoffset);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    switch(button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
             EventMgr::getInstance()->Triger(lastX, lastY, action);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            cout<<"Mosue right button"<<endl;
            break;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    SceneMgr::getInstance()->ProcessMouseScroll(xoffset, yoffset);
}
