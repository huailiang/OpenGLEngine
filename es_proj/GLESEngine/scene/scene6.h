//
//  scene6.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/26/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene6_h
#define scene6_h
#include <cmath>
#include "scene.h"

class Scene6 : public Scene
{
    
public:
    
    ~Scene6()
    {
        SAFE_DELETE(shader);
        DELETE_TEXTURE(m_backgroundTextureId);
        SAFE_DELETE(vrShader);
        glDeleteVertexArrays(1, &quadVao);
        glDeleteBuffers(1, &quadVbo);
        glDeleteVertexArrays(1, &vrVao);
        glDeleteBuffers(1, &vrVBo);
    }
    
    virtual bool isVRScene() { return true; }
    
    int getType() { return TY_Scene6; }
    
    std::string getSkybox() { return ""; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f); }
    
    void InitLight() { light = new DirectLight(vec3(1.0f), vec3(0.0f, 0.0f, -2.0f)); }
    
    void InitScene()
    {
        glGenTextures(1, &m_backgroundTextureId);
        glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // This is necessary for non-power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        shader = new Shader("debug.vs", "debug.fs");
        shader->attach("_FLIP_Y_");
        InitFullQuad(&quadVao, &quadVbo, shader);
        
        vrShader = new Shader("debug.vs","debug.fs");
        vrShader->attach("_AR_");
        InitCube(&vrVao, &vrVBo, vrShader);
        vrShader->compile();
    }
    
    void Clean()
    {
        glDepthMask(true);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void InitialVR(float width, float height,const Matrix33& intrinsic)
    {
        proj = glm::mat4(0);
        float fx = intrinsic.data[0], n= -0.01f, f = -100.0;
        proj[0][0] = 2.0 * fx / width;
        proj[1][1] = 2.0 * fx / height;
        proj[2][2] = (f + n) / (n - f);
        proj[2][3] = -1.0;
        proj[3][2] = 2.0 * f * n / ( f - n);
        
        reverse = glm::mat4(1);
        reverse[1][1] = -1;
        reverse[2][2] = -1;
    }
    
    void SetCameraFrame(const BGRAVideoFrame& frame)
    {
        this->camFrame = frame;
    }
    
    void DrawBackground()
    {
        shader->use();
        GLsizei width = (GLsizei)camFrame.width;
        GLsizei height = (GLsizei)camFrame.height;
        glActiveTexture(GL_TEXTURE0);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, camFrame.data);
        glBindVertexArray(quadVao);
        glDrawArrays(DRAW_MODE, 0, 6);
        glBindVertexArray(0);
    }
    
    void DrawAR(const std::vector<Transformation>& transforms)
    {
        Clean();
        DrawBackground();
        loop(transforms.size())
        {
            glm::mat4 view = transforms[i].getMat44(); //camera's position & rotation
            view = reverse * view;
            vrShader->use();
            vrShader->setMat4("view",  view);
            vrShader->setMat4("proj", proj);
            glBindVertexArray(vrVao);
            glDrawArrays(DRAW_MODE, 0, 36);
            glBindVertexArray(0);
        }
    }

private:
    Shader* shader;
    Shader* vrShader;
    GLuint m_backgroundTextureId;
    GLuint quadVao, quadVbo;
    GLuint vrVao, vrVBo;
    glm::mat4 proj, reverse;
    BGRAVideoFrame camFrame;
};


#endif /* scene6_h */
