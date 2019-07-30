//
//  skybox.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/29/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "skybox.hpp"
#include "asset.hpp"
#ifdef _GLES_
#include "FilePath.h"
#else
#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"
#endif
#include "texmgr.hpp"

namespace engine
{

    Skybox::Skybox(Camera* camera,std::string name,bool hdr)
    {
        this->camera=camera;
        this->name = name;
        this->hdr = hdr;
        skyShader = new Shader("cube.vs","cube.fs");
        if (hdr)
        {
            equirectangularToCubemapShader = new Shader("cube.vs","ibl/equirect2cube.fs");
            irradianceShader = new Shader("cube.vs", "ibl/irradiance_convolution.fs");
            prefilterShader = new Shader("cube.vs","ibl/prefilter.fs");
            brdfShader = new  Shader("ibl/brdf.vs", "ibl/brdf.fs");
        }
        init_buff();
        init_tex();
    }

    Skybox::~Skybox()
    {
        SAFE_DELETE(skyShader);
        glDeleteBuffers(1,&vbo);
        glDeleteVertexArrays(1,&vao);
        if(hdr)
        {
            SAFE_DELETE(brdfShader);
            SAFE_DELETE(equirectangularToCubemapShader);
            SAFE_DELETE(prefilterShader);
            SAFE_DELETE(irradianceShader);
            DELETE_TEXTURE(hdrTexture);
            glDeleteTextures(1, &envCubemap);
            glDeleteTextures(1, &irradianceMap);
            glDeleteTextures(1, &brdfLUTTexture);
            glDeleteTextures(1, &prefilterMap);
            
        }
        else
        {
            DELETE_TEXTURE(skyTexture);
        }
    }

    
    void Skybox::Draw()
    {
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth  buffer's content
        
        skyShader->use();
        glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
        skyShader->setMat4("view", view);
        skyShader->setMat4("projection", camera->GetProjMatrix());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
        glBindVertexArray(vao);
        glDrawArrays(DRAW_MODE, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default        
    }

    void Skybox::init_tex()
    {
        if (hdr) { //hdr 即envmap 只有一张图EquirectangularMap 需要转换为cubemap
            std::string path ="textures/hdr/"+name;
            Texture(path.c_str(), _HDR, &hdrTexture, true, GL_REPEAT, false);
            Equirect2Cube();
        }
        else
        {
            std::string path = "textures/skybox/"+name+"/";
            Texture(path.c_str(), &skyTexture);
        }
    }
    
    void Skybox::init_buff()
    {
        float vertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f,  -1.0f, -1.0f,
            1.0f,  -1.0f, -1.0f,
            1.0f,   1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,   1.0f,  1.0f,
            1.0f,   1.0f,  1.0f,
            1.0f,  -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            
            -1.0f,  1.0f, -1.0f,
            1.0f,   1.0f, -1.0f,
            1.0f,   1.0f,  1.0f,
            1.0f,   1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f,  -1.0f, -1.0f,
            1.0f,  -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f,  -1.0f,  1.0f
        };
        
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    
    void Skybox::MakeCube(unsigned int* map, int size, bool mipmap)
    {
        glGenTextures(1, map);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *map);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR: GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    
    void Skybox::Equirect2Cube()
    {
        if(hdr)
        {
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glGenFramebuffers(1, &captureFBO);
            glGenRenderbuffers(1, &captureRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
            
            glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
            glm::mat4 captureViews[] =
            {
                glm::lookAt(glm::vec3(0), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                glm::lookAt(glm::vec3(0), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                glm::lookAt(glm::vec3(0), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
                glm::lookAt(glm::vec3(0), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
                glm::lookAt(glm::vec3(0), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                glm::lookAt(glm::vec3(0), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
            };
            
            GenerateEnvmap(captureViews, captureProjection);
            GenerateIrradiance(captureViews, captureProjection);
            GeneratePrefilter(captureViews, captureProjection);
            GenerateLut();
            
            glDeleteFramebuffers(1, &captureFBO);
            glDeleteRenderbuffers(1, &captureRBO);
            skyTexture = envCubemap;
            glViewport(0, 0, RENDER_WIDTH, RENDER_HEIGTH);
        }
        else
        {
            std::cerr<<"hdrTexture failed"<<std::endl;
        }
    }
    
    void Skybox::GenerateEnvmap(glm::mat4 captureViews[], const glm::mat4 captureProjection)
    {
        glViewport(0, 0, 512, 512);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        MakeCube(&envCubemap, 512, true);
        equirectangularToCubemapShader->use();
        equirectangularToCubemapShader->setInt("equirectangularMap", 0);
        equirectangularToCubemapShader->setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        
        for (unsigned int i = 0; i < 6; ++i)
        {
            equirectangularToCubemapShader->setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderCube();
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void Skybox::GenerateIrradiance(glm::mat4 captureViews[], const glm::mat4 captureProjection)
    {
        MakeCube(&irradianceMap, 32, false);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
        irradianceShader->use();
        irradianceShader->setInt("environmentMap", 0);
        irradianceShader->setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        glViewport(0,0,32,32);
        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader->setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void Skybox::GeneratePrefilter(glm::mat4 captureViews[], const glm::mat4 captureProjection)
    {
        MakeCube(&prefilterMap, 128, true);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        prefilterShader->use();
        prefilterShader->setInt("environmentMap", 0);
        prefilterShader->setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth  = 128 * std::pow(0.5, mip);
            unsigned int mipHeight = 128 * std::pow(0.5, mip);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);
            
            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterShader->setFloat("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader->setMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                RenderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void Skybox::GenerateLut()
    {
        glGenTextures(1, &brdfLUTTexture);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);
        TexMgr::getInstance()->SetTextureFormat(GL_TEXTURE_2D, GL_LINEAR, GL_CLAMP_TO_EDGE);
        
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
        
        glViewport(0, 0, 512, 512);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Skybox::RenderCube()
    {
        GLuint cubeVAO, cubeVBO;
        InitCube(&cubeVAO, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glDrawArrays(DRAW_MODE, 0, 36);
        glBindVertexArray(0);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteVertexArrays(1, &cubeVAO);
    }
    
    void Skybox::RenderQuad()
    {
        GLuint quadVAO, quadVBO;
        InitFullQuad(&quadVAO, &quadVBO, brdfShader);
        brdfShader->use();
        glBindVertexArray(quadVAO);
        glDrawArrays(DRAW_MODE, 0, 6);
        glBindVertexArray(0);
        glDeleteBuffers(1, &quadVBO);
        glDeleteVertexArrays(1, &quadVAO);
    }

}
