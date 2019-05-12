//
//  main.cpp
//  Created by 彭怀亮 on 5/8/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "std/stb_image.h"
#include "std/camera.h"
#include "std/shader.h"
#include "std/texture.h"
#include "std/model.h"
#include "std/light.h"
#include "terrain.h"
#include "profile.h"
#include "ttfont.h"
#include "std/const.h"

using namespace std;
using namespace glm;

float deltatime,lastTime;
Camera camera(vec3(0.0f,0.0f,3.0f));

#define _SpotLight_
Light* light;


void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

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
    glfwSetScrollCallback(window, scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    LightShader shader("shader/light.vs","shader/light.fs",nullptr,vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 0.5f),vec3(0.5f, 0.5f, 0.5f),32.0f);
    LightShader mShader("shader/model.vs", "shader/model.fs",   nullptr,vec3(1.0f, 1.0f, 1.0f), vec3(2.0f, 2.0f, 1.5f),vec3(0.5f, 0.5f, 0.5f),32.0f);
    Shader oShader("shader/model.vs", "shader/outline.fs");
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  -2.0f),
        glm::vec3( 2.0f,  1.0f, -4.0f),
    };
    
    //cube
    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    unsigned int texture1, texture2;
    TTexture tex1("resources/textures/container.jpg", &texture1);
    TTexture tex2("resources/textures/awesomeface.png", &texture2);

     // don't forget to activate/use the shader before setting uniforms!
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    shader.setVec3("viewPos", camera.Position);
    shader.ApplyLight();
    
    //light
#ifdef _SpotLight_
    light = new SpotLight(vec3(1.0f),vec3(0,0,-1),vec3(0,0,2.0f),vec3(1.0f,0.09f,0.032f),cos(radians(7.5f)),cos(radians(8.5f)));
#else
#ifdef _PointLight_
    light = new PointLight(vec3(1.0f),vec3(0,0,-1),vec3(0,0,4),vec3(1.0f,0.09f,0.032f));
#else
     light = new DirectLight(vec3(1.0f),vec3(1.0f,0.0f,0.0f));
#endif
#endif

    Terrain terrain;
    TTFont font;
    Model Model("resources/objects/nanosuit/nanosuit.obj");
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        shader.use();
        float timeValue = glfwGetTime();
        deltatime  = timeValue-lastTime;
        lastTime= timeValue;
        float scale = sin(timeValue) / 3.0f + 0.66f;
        shader.setFloat("scale", scale);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        mat4 view = camera.GetViewMatrix();
        mat4 proj = camera.GetProjMatrix();
        shader.setMat4("view", view);
        shader.setMat4("projection", proj);
        light->Attach(&shader);
        glBindVertexArray(vao);
        glStencilMask(0x00);
        for (unsigned int i = 0; i < 2; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = translate(model, cubePositions[i]);
            float angle = 64 * i * timeValue;
            model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        Model.Draw(&mShader,&camera, light, vec3(-1.2f, -0.5f, -1.5f), vec3(0.12f, 0.12f, 0.12f), -16*timeValue);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        Model.Draw(&oShader,&camera, light, vec3(-1.2f, -0.5f, -1.5f), vec3(0.123f, 0.121f, 0.121f), -16*timeValue);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_STENCIL_BUFFER_BIT);
        
        terrain.Draw(proj * view * mat4(1));
        
        float fps = 1.0f / deltatime;
        font.RenderText("FPS: "+to_string_with_precision(fps,4), 740, 580, 0.5f, vec3(1.0f,0.0f,0.0f));
        font.RenderText("@copyright penghuailiang", 20, 20, 1.0f, vec3(1.0f,1.0f,0.0f));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A)== GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, 0.02f);
    if (glfwGetKey(window, GLFW_KEY_D)== GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, 0.02f);
    if (glfwGetKey(window, GLFW_KEY_W)== GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, 0.02f);
    if (glfwGetKey(window, GLFW_KEY_S)== GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, 0.02f);
    if ( glfwGetKey(window, GLFW_KEY_LEFT)== GLFW_PRESS)
        light->UpdateX(-0.01f);
    if ( glfwGetKey(window, GLFW_KEY_RIGHT)== GLFW_PRESS)
        light->UpdateX(0.01f);
    if ( glfwGetKey(window, GLFW_KEY_UP)== GLFW_PRESS)
        light->UpdateY(0.01f);
    if ( glfwGetKey(window, GLFW_KEY_DOWN)== GLFW_PRESS)
        light->UpdateY(-0.01f);
    if (glfwGetKey(window, GLFW_KEY_SPACE)== GLFW_PRESS)
    {
        float timeValue = glfwGetTime();
        float camX =  10.0f * sin(timeValue);
        float camZ = 10.0f * cos(timeValue);
        camera.RotateAt(vec3(camX,0,camZ), vec3(0,0,0));
    }
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
    float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
