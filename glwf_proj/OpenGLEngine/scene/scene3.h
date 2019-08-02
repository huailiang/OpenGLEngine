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
#include <limits>

class Scene3 : public Scene
{
    
public:
    
    ~Scene3()
    {
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &planeVBO);
        glDeleteBuffers(1, &cubeVBO);
        SAFE_DELETE(shader);
    }
    
    glm::vec3 getCameraPos() { return glm::vec3(1.0f, 0.4f, 4.0f); }

    int getType() { return TY_Scene3; }
    
    void InitLight() { light = new DirectLight(color_white, vec3(1.0f,-2.0f,2.0f)); }
    
    void InitScene()
    {
        debug = true;
        shader  = new LightShader("shadow.vs", "shadow.fs");
        shader->attach("CASCADES");
        shader->attach("DEBUG");
        InitPlane(&planeVAO, &planeVBO, shader);
        InitCube(&cubeVAO, &cubeVBO);
        Texture("textures/wood", _PNG, &woodTexture, true, GL_REPEAT, true); // open mipmap
        shader->use();
        ApplyCamera(shader);
        shader->setInt("diffuseTexture", 0);
        shader->setInt("shadowMap", 1);
    }
    
    void InitShadow()
    {
        shadow = new CascadedShadow(SHADOW_WIDTH, shader);
        CascadedShadow *cShadow = static_cast<CascadedShadow*>(shadow);
        debugTexID = cShadow->map[0];
        dist[0] = 0.4;
        dist[1] = 5.0;
        dist[2] = 10.0;
        dist[3] = 45.0;
        loop(CASCACDE_NUM) {
            vec4 vpos(0.0, 0.0, -dist[i+1], 1.0);
            mat4 proj = camera->GetProjMatrix();
            vec4 clipPos = proj * vpos;
            char Name[128] = { 0 };
            snprintf(Name, sizeof(Name), "gCascadeEndClipSpace[%d]", i);
            shader->setFloat(Name, clipPos.z);
        }
    }
    
    void CalShadowMatrix(int i)
    {
        float aspect = SCR_WIDTH / (float)SCR_HEIGHT;
        float tanHFov = tan(glm::radians(camera->FOV/2.0));
        float yn = dist[i] * tanHFov;
        float xn = yn * aspect;
        float yf = dist[i+1] * tanHFov;
        float xf = yf * aspect;
        vec4 frustumCorners[] = {
            vec4(xn,   yn, -dist[i], 1.0),
            vec4(-xn,  yn, -dist[i], 1.0),
            vec4(xn,  -yn, -dist[i], 1.0),
            vec4(-xn, -yn, -dist[i], 1.0),
            
            vec4(xf,   yf, -dist[i + 1], 1.0),
            vec4(-xf,  yf, -dist[i + 1], 1.0),
            vec4(xf,  -yf, -dist[i + 1], 1.0),
            vec4(-xf, -yf, -dist[i + 1], 1.0)
        };
        float max = 2e31;
        float minX = max;
        float maxX = -max;
        float minY = max;
        float maxY = -max;
        float minZ = max;
        float maxZ = -max;
        mat4 camInv = glm::inverse(camera->GetViewMatrix());
        mat4 lightView = light->getViewMatrix();
        loop0j(NUM_FRUSTUM_CORNERS) {
            vec4 world = camInv * frustumCorners[j];
            vec4 light = lightView * world;
            minX = min(minX, light.x);
            maxX = max(maxX, light.x);
            minY = min(minY, light.y);
            maxY = max(maxY, light.y);
            minZ = min(minZ, light.z);
            maxZ = max(maxZ, light.z);
        }
        mat4 proj = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        lightMatrix = proj * lightView;
    }
    
    void DrawShadow()
    {
        Scene::DrawShadow();
        CascadedShadow *cShadow = static_cast<CascadedShadow*>(shadow);
        Shader* shader = shadow->getShader();
        loop(CASCACDE_NUM) {
            CalShadowMatrix(i);
            cShadow->BindFbo(i, lightMatrix);
            ClearDepth();
            RenderCubes(shader);
        }
    }
    
    void DrawScene()
    {
        shader->use();
        light->Apply(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        CascadedShadow *cShadow = static_cast<CascadedShadow*>(shadow);
        mat4 mtx[CASCACDE_NUM];
        loop(CASCACDE_NUM)
        {
            CalShadowMatrix(i);
            mtx[i] = lightMatrix;
        }
        cShadow->BindRender(mtx);
        RenderScene(shader);
    }
    
    void RenderScene(const Shader *shader)
    {
        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);
        glBindVertexArray(planeVAO);
        glDrawArrays(DRAW_MODE, 0, 6);
        
        RenderCubes(shader);
    }
    
    void RenderCubes(const Shader *shader)
    {
        renderCube(glm::vec3(0.0f, 0.0, -3.6), 0.5f, 0.0f, shader);
        renderCube(glm::vec3(2.0f, 0.0, -0.2), 0.5f, 0.0f, shader);
        renderCube(glm::vec3(-1.f, 0.0, -2.0), 0.25f, 16 * timeValue, shader);
        renderCube(glm::vec3(1.5f, 0.1, -8.0), 0.36f, 16 * timeValue, shader);
    }
    
    void renderCube(glm::vec3 pos, float scale, float angle, const Shader *shader)
    {
        glBindVertexArray(cubeVAO);
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(angle), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(scale));
        shader->setMat4("model", model);
        glDrawArrays(DRAW_MODE, 0, 36);
        glBindVertexArray(0);
    }

    void OnKeyboard(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            CascadedShadow *cShadow = static_cast<CascadedShadow*>(shadow);
            debugTexID = cShadow->map[0];
        }
        else if(glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            CascadedShadow *cShadow = static_cast<CascadedShadow*>(shadow);
            debugTexID = cShadow->map[1];
        }
        else if(glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            CascadedShadow *cShadow = static_cast<CascadedShadow*>(shadow);
            debugTexID = cShadow->map[2];
        }
    }
    
private:
    LightShader *shader = nullptr;
    GLuint woodTexture;
    GLuint planeVBO, planeVAO;
    GLuint cubeVAO, cubeVBO;
    
    float dist[4];
    
};

#endif /* scene3_h */
