//
//  skybox.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/29/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "skybox.h"
#include "asset.h"
#ifdef _GLES_
#include "FilePath.h"
#else
#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"
#endif
#include "texmgr.h"

namespace engine
{

    Skybox::Skybox(Camera* camera,std::string name,bool hdr)
    {
        this->camera=camera;
        this->name = name;
        shader = new Shader("cube.vs","cube.fs");
        if (hdr)
        {
            equirectangularToCubemapShader =new Shader("cube.vs","equirect2cube.fs");
            irradianceShader = new Shader("cube.vs", "irradiance_convolution.fs");
        }
        init_tex(hdr);
        init_buff();
    }

    Skybox::~Skybox()
    {
        SAFE_DELETE(shader);
        SAFE_DELETE(equirectangularToCubemapShader);
        glDeleteBuffers(1,&vbo);
        glDeleteVertexArrays(1,&vao);
        DELETE_TEXTURE(cubemapTexture);
        DELETE_TEXTURE(hdrTexture);
        DELETE_TEXTURE(envCubemap);
        DELETE_TEXTURE(irradianceMap);
    }


    void Skybox::Draw()
    {
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        shader->use();
        glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
        shader->setMat4("view", view);
        shader->setMat4("projection", camera->GetProjMatrix());
        // skybox cube
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(DRAW_MODE, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default        
    }

    void Skybox::init_tex(bool hdr)
    {
        if (hdr) { //hdr 即envmap 只有一张图EquirectangularMap 需要转换为cubemap
            std::string path ="textures/hdr/"+name;
            Texture(path.c_str(), HDR, &hdrTexture,true, GL_REPEAT,false);
            Equirect2Cube();
        }
        else
        {
            std::string path = "textures/skybox/"+name+"/";
            Texture(path.c_str(), &cubemapTexture);
        }
    }
    
    void Skybox::MakeCube(unsigned int* envCubemap, int size)
    {
        glGenTextures(1, envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    
    void Skybox::Equirect2Cube()
    {
        if(hdrTexture)
        {
            unsigned int captureFBO;
            unsigned int captureRBO;
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
                        
            MakeCube(&envCubemap, 512);
            equirectangularToCubemapShader->use();
            equirectangularToCubemapShader->setInt("equirectangularMap", 0);
            equirectangularToCubemapShader->setMat4("projection", captureProjection);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, hdrTexture);
            
            glViewport(0, 0, 512, 512);
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            for (unsigned int i = 0; i < 6; ++i)
            {
                equirectangularToCubemapShader->setMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                RenderCube();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            MakeCube(&irradianceMap, 32);
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
            irradianceShader->use();
            irradianceShader->setInt("environmentMap", 0);
            irradianceShader->setMat4("projection", captureProjection);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
            glViewport(0,0,32,32);
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            for (unsigned int i = 0; i < 6; ++i)
            {
                irradianceShader->setMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                RenderCube();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            cubemapTexture = envCubemap;
            glViewport(0, 0, RENDER_WIDTH, RENDER_HEIGTH);
            
        }
        else
        {
            std::cerr<<"hdrTexture failed"<<std::endl;
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
    
    
    void Skybox::RenderCube()
    {
        GLuint cubeVAO, cubeVBO;
        InitCube(cubeVAO, cubeVBO);
        glBindVertexArray(cubeVAO);
        glDrawArrays(DRAW_MODE, 0, 36);
        glBindVertexArray(0);
    }

}
