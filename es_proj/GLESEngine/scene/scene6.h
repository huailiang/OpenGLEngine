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
#include "arscene.h"

class Scene6 : public ARScene
{
    
public:
    
    ~Scene6()
    {
        SAFE_DELETE(shader);
        DELETE_TEXTURE(m_backgroundTextureId);
        SAFE_DELETE(arShader);
        glDeleteVertexArrays(1, &quadVao);
        glDeleteBuffers(1, &quadVbo);
        glDeleteVertexArrays(1, &arVao);
        glDeleteBuffers(1, &arVBo);
    }
    
    int getType() { return TY_Scene6; }
    
    void InitLight() { light = new DirectLight(vec3(1.0f), vec3(0.0f, 0.0f, -2.0f)); }
    
    void InitScene()
    {
        ARScene::InitScene();
        
        arShader = new Shader("debug.vs","debug.fs");
        arShader->attach("_AR_");
        InitCube(&arVao, &arVBo, arShader);
        arShader->compile();
    }
    
    
    void DrawScene()
    {
        DrawBackground();
        ARScene::DrawAR(transforms);
        loop(transforms.size())
        {
            glm::mat4 view = transforms[i].getMat44(); //camera's position & rotation
            view = reverse * view;
            arShader->use();
            arShader->setMat4("view",  view);
            arShader->setMat4("proj", proj);
            glBindVertexArray(arVao);
            glDrawArrays(DRAW_MODE, 0, 36);
            glBindVertexArray(0);
        }
    }

private:
    Shader* arShader;
    GLuint arVao, arVBo;
    
};


#endif /* scene6_h */
