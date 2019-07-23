//
//  vrscene.h
//  GLESEngine
//
//  Created by 彭怀亮 on 7/22/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef vrscene_h
#define vrscene_h

#include "scene.h"
#include "IARInterface.h"

class ARScene: public Scene
{
    
public:
    
    virtual ~ARScene()
    {
        SAFE_DELETE(shader);
        DELETE_TEXTURE(m_backgroundTextureId);
        glDeleteVertexArrays(1, &quadVao);
        glDeleteBuffers(1, &quadVbo);
    }
    
    virtual bool isARScene() { return true; }
    
    virtual std::string getSkybox() { return ""; }
    
    virtual glm::vec3 getCameraPos() { return glm::vec3(0.0f); }
    
    virtual bool drawShadow() { return false; }
    
    virtual void InitScene()
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
    }
    
    void InitialVR(float width, float height,const glm::mat3& intrinsic)
    {
        proj = glm::mat4(0);
        float fx = intrinsic[0][0], n= -0.01f, f = -100.0;
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
        this->transforms = transforms;
    }
    
    virtual void Process(IARInterface* ar) { }
    
    
protected:
    BGRAVideoFrame camFrame;
    std::vector<Transformation> transforms;
    Shader* shader;
    GLuint m_backgroundTextureId;
    GLuint quadVao, quadVbo;
    glm::mat4 proj, reverse;
    
};

#endif /* vrscene_h */
