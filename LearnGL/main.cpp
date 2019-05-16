//
//  main.cpp
//  Created by 彭怀亮 on 5/8/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//
#include "common.h"
#include "std/camera.h"
#include "std/shader.h"
#include "std/texture.h"
#include "std/model.h"
#include "std/skybox.h"
#include "std/light.h"
#include "gui/uimgr.h"
#include "gui/eventmgr.h"
#include "gui/label.h"
#include "profile.h"
#include "terrain.h"
#include "screen.h"
#include "scene/scenemgr.h"

using namespace std;
using namespace glm;

float deltatime,lastTime;
bool normal;

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

UIManager UIManager::instance;
EventMgr EventMgr::instance;
TTFont TTFont::instance;
SceneMgr SceneMgr::instance;

void Test(UIEvent* contex)
{
    Label* label = dynamic_cast<Label*>(contex);
    cout<<"callback "<<label->getText()<<endl;
}

int main(int argc, const char * argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnGL", NULL, NULL);
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
   
    Screen screen;

    TTFont::getInstance()->initial();
    SceneMgr::getInstance()->Init();

    glEnable(GL_DEPTH_TEST);
    Shader oShader("shader/model.vs", "shader/outline.fs");
    Shader sShader("shader/screen.vs","shader/screen.fs");


    screen.Bind(true);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Label(vec2(120,20),vec3(1.0f), 1, "offline screen render");
    screen.Bind(false);
    
//    Label lb(vec2(20,20), vec3(1.0f,1.0f,0.0f));
//    Label lb2(vec2(740, 580), vec3(1,0,0), 0.5f);
//    lb2.RegistCallback(Test);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        float timeValue = glfwGetTime();
        deltatime  = timeValue-lastTime;
        lastTime= timeValue;
        SceneMgr::getInstance()->Draw(deltatime);
        screen.RTDraw();
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
    if ( glfwGetKey(window, GLFW_KEY_N)== GLFW_PRESS)
        normal = !normal;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    cout<<"view changed"<<endl;
    glViewport(0,0,width,height);
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
        case GLFW_MOUSE_BUTTON_MIDDLE:
            cout<<"Mosue middle button "<<endl;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            cout<<"Mosue right button"<<endl;
            break;
        default:
            return;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    SceneMgr::getInstance()->ProcessMouseScroll(xoffset, yoffset);
}
