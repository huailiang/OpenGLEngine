//
//  shadow.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 7/30/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "shadow.hpp"
#include "texmgr.hpp"

namespace engine
{
  
    Shadow::Shadow(uint w, Shader* rsh)
    {
        fbo = 0;
        width = w;
        dShader  = new Shader("depth.vs","depth.fs");
        rShader = rsh;
    }

    Shadow::~Shadow()
    {
        if(fbo != 0) glDeleteFramebuffers(1, &fbo);
        SAFE_DELETE(dShader);
    }

    Shader* Shadow::getShader() const
    {
        return dShader;
    }

    GeneralShadow::GeneralShadow(uint width, Shader* rsh)
    : Shadow(width, rsh)
    {
        Init();
        rsh->use();
        rsh->setInt("shadowMap", SHADOW_TEXTURE_UNIT);
    }

    GeneralShadow::~GeneralShadow()
    {
        glDeleteTextures(1, &map);
    }

    bool GeneralShadow::Init()
    {
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &map);
        glBindTexture(GL_TEXTURE_2D, map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, map, 0);
        
        // Disable writes to the color buffer
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        
        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        
        if (Status != GL_FRAMEBUFFER_COMPLETE) {
            printf("FB error, status: 0x%x\n", Status);
            return false;
        }
        return true;
    }

    void GeneralShadow::BindFbo(const glm::mat4 lightMatrix)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
        dShader->use();
        dShader->setMat4("lightSpaceMatrix", lightMatrix);
    }

    void GeneralShadow::BindRender(const glm::mat4 lightMatrix[])
    {
        rShader->use();
        rShader->setMat4("lightSpaceMatrix", lightMatrix[0]);
        glActiveTexture(SHADOW_TEXTURE_UNIT);
        glBindTexture(GL_TEXTURE_2D, map);
    }

    CascadedShadow::CascadedShadow(uint width, Shader* rsh)
    : Shadow(width, rsh)
    {
        ZERO_MEM(map);
        Init();
        
        rsh->attach("CASCADES");
        rsh->use();
        rsh->setInt("gShadowMap[0]", CASCACDE_SHADOW_TEXTURE_UNIT0_INDEX);
        rsh->setInt("gShadowMap[1]", CASCACDE_SHADOW_TEXTURE_UNIT1_INDEX);
        rsh->setInt("gShadowMap[2]", CASCACDE_SHADOW_TEXTURE_UNIT2_INDEX);
    }

    CascadedShadow::~CascadedShadow()
    {
        int num = ARRAY_SIZE_IN_ELEMENTS(map);
        glDeleteTextures(num, map);
    }

    bool CascadedShadow::Init()
    {
        glGenFramebuffers(1, &fbo);
        glGenTextures(CASCACDE_NUM, map);
        
        for (uint i = 0 ; i < CASCACDE_NUM ; i++) {
            glBindTexture(GL_TEXTURE_2D, map[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, map[0], 0);
        
        // Disable writes to the color buffer
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        
        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (Status != GL_FRAMEBUFFER_COMPLETE) {
            printf("FB error, status: 0x%x\n", Status);
            return false;
        }
        return  true;
    }

    void CascadedShadow::BindFbo(uint CascadeIndex,const glm::mat4 lightMatrix)
    {
        assert(CascadeIndex < CASCACDE_NUM);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, map[CascadeIndex], 0);
        dShader->use();
        dShader->setMat4("lightSpaceMatrix", lightMatrix);
    }

    void CascadedShadow::BindRender(const glm::mat4 lightMatrix[])
    {
        rShader->use();
        loop(CASCACDE_NUM) {
            char Name[128] = { 0 };
            snprintf(Name, sizeof(Name), "lightSpaceMatrix[%d]", i);
            rShader->setMat4(Name, lightMatrix[i]);
        }
        
        glActiveTexture(CASCACDE_SHADOW_TEXTURE_UNIT0);
        glBindTexture(GL_TEXTURE_2D, map[0]);
        
        glActiveTexture(CASCACDE_SHADOW_TEXTURE_UNIT1);
        glBindTexture(GL_TEXTURE_2D, map[1]);
        
        glActiveTexture(CASCACDE_SHADOW_TEXTURE_UNIT2);
        glBindTexture(GL_TEXTURE_2D, map[2]);
    }

}

