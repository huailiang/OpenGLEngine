//
//  scene6.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/26/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene6_h
#define scene6_h

#include "scene.h"

class Scene6 : public Scene
{
    
public:
    
    ~Scene6()
    {
        SAFE_DELETE(shader);
        DELETE_TEXTURE(m_backgroundTextureId);
        SAFE_DELETE(lshader);
        SAFE_DELETE(vrShader);
        SAFE_DELETE(mat);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &quadVao);
        glDeleteBuffers(1, &quadVbo);
        glDeleteVertexArrays(1, &vrVao);
        glDeleteBuffers(1, &vrVBo);
    }
    
    virtual bool isVRScene() { return true; }
    
    int getType() { return TY_Scene6; }
    
    std::string getSkybox() { return ""; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f); }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(0.0f,0.0f,-2.0f));
    }
    
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
        vrShader->attach("_VR_");
        vrShader->attach("VERT_TYPE","0x002");
        vrShader->compile();
        
        float vertices[] = {
            -0.50f, -0.50f,
            0.50f,  -0.50f,
            -0.50f,  0.50f,
            0.50f,   0.50f,
        };
        
        glGenVertexArrays(1, &vrVao);
        glGenBuffers(1, &vrVBo);
        glBindVertexArray(vrVao);
        glBindBuffer(GL_ARRAY_BUFFER, vrVBo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glBindVertexArray(0);
        
        MeshData* cube =(MeshData*)InitCube(&vao, &vbo);
        mat= new Material(cube);
        lshader = new LightShader("light.vs","light.fs");
        lshader->attach(light->getMacro().c_str());
        mat->BindVert(lshader);
        lshader->use();
        mat->SetFloat("scale", 1);
        mat->SetTexture("texture1", "textures/container", _JPG);
        mat->SetTexture("texture2", "textures/awesomeface", _PNG);
        ApplyCamera(mat);
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
    
    void InitialVR(int width, int height,const Matrix33& intrinsic)
    {
        this->width = width;
        this->height = height;
        this->cameraMatrix = intrinsic;
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
        if(transforms.size())
        {
            glm::mat4 projectionMatrix;
            BuildProjection(projectionMatrix);

            loop(transforms.size())
            {
                std::cout<<"pos:"<<transforms[i].t()<<std::endl;
                glm::mat4 model = transforms[i].getMat44(); //position & rotation
                vrShader->use();
                vrShader->setMat4("model", model);
                vrShader->setMat4("proj", projectionMatrix);
                glBindVertexArray(vrVao);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);
                
                mat->Draw();
                light->Apply(mat);
                glBindVertexArray(vao);
                model = glm::scale(model, vec3(0.5));
                mat->SetMat4("model", model);
                glDrawArrays(DRAW_MODE, 0, 36);
                glBindVertexArray(0);

            }
        }
    }
    
    void DrawCube()
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = translate(model, vec3(0,0,-3));
        model = glm::rotate(model, glm::radians(GetRuntime()), vec3(1.0f, 0.3f, 0.5f));
        model = glm::scale(model, vec3(0.2f));
        mat->Draw();
        light->Apply(mat);
        glBindVertexArray(vao);
        mat->SetMat4("model", model);
        glDrawArrays(DRAW_MODE, 0, 36);
        glBindVertexArray(0);

    }
    
    void BuildProjection(glm::mat4& projectionMatrix)
    {
        float near = 0.01;  // Near clipping distance
        float far  = 100;  // Far clipping distance
        
        float f_x = cameraMatrix.data[0]; // Focal length in x axis
        float f_y = cameraMatrix.data[4]; // Focal length in y axis (usually the same?)
        
        projectionMatrix[0][0] = - 2.0 * f_x / this->width;
        projectionMatrix[0][1] = 0.0;
        projectionMatrix[0][2] = 0.0;
        projectionMatrix[0][3] = 0.0;
        
        projectionMatrix[1][0] = 0.0;
        projectionMatrix[1][1] = 2.0 * f_y / this->height;
        projectionMatrix[1][2] = 0.0;
        projectionMatrix[1][3] = 0.0;
        
        projectionMatrix[2][0] = 0.0;
        projectionMatrix[2][1] = 0.0;
        projectionMatrix[2][2] = -(far + near) / ( far - near );
        projectionMatrix[2][3] = -1.0;
        
        projectionMatrix[3][0] = 0.0;
        projectionMatrix[3][1] = 0.0;
        projectionMatrix[3][2] = -2.0 * far * near / ( far - near );
        projectionMatrix[3][3] = 0.0;
    }


private:
    Shader* shader;
    Shader* vrShader;
    int width, height;
    Matrix33 cameraMatrix;
    GLuint m_backgroundTextureId;
    GLuint quadVao, quadVbo;
    GLuint vrVao, vrVBo;
    
    Material* mat = nullptr;
    GLuint vbo, vao;
    LightShader* lshader;
    
    BGRAVideoFrame camFrame;
};


#endif /* scene6_h */
