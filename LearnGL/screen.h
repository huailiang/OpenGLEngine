//
//  screen.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/13/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef screen_h
#define screen_h


#include "common.h"
#include "std/shader.h"

class Screen
{
    
public:
    Screen()
    {
        shader = new Shader("screen.vs","screen.fs");
        InitQuad();
        InitFrame();
    }
    
    ~Screen()
    {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteFramebuffers(1, &framebuffer);
    }
    
    void RTDraw()
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
        glDisable(GL_DEPTH_TEST);
        shader->use();
        glViewport(0,0,RENDER_WIDTH,RENDER_HEIGTH);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    
    void Bind(bool bind)
    {
        if(bind)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
    
private:
    void InitFrame()
    {
        shader->use();
        shader->setInt("screenTexture", 0);
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
            initial = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        initial = true;
    }
    
    void InitQuad()
    {
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }
    
    unsigned int rbo;
    unsigned int textureColorbuffer;
    unsigned int framebuffer;
    unsigned int quadVAO, quadVBO;
    Shader* shader;
    bool initial;
};

#endif /* screen_h */
