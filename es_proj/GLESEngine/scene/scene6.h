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
#include "vrscene.h"

class Scene6 : public VRScene
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
    
    int getType() { return TY_Scene6; }
    
    void InitLight() { light = new DirectLight(vec3(1.0f), vec3(0.0f, 0.0f, -2.0f)); }
    
    void InitScene()
    {
        VRScene::InitScene();
        
        vrShader = new Shader("debug.vs","debug.fs");
        vrShader->attach("_AR_");
        InitCube(&vrVao, &vrVBo, vrShader);
        vrShader->compile();
    }
    
    void DrawAR(const std::vector<Transformation>& transforms)
    {
        VRScene::DrawAR(transforms);
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
    Shader* vrShader;
    GLuint vrVao, vrVBo;
    
};


#endif /* scene6_h */
