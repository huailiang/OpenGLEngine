//
//  scene6.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/26/19.
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
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
        DELETE_TEXTURE(albedo);
        DELETE_TEXTURE(normal);
        DELETE_TEXTURE(metallic);
        DELETE_TEXTURE(roughness);
        DELETE_TEXTURE(ao);
    }
    
    int getType() { return TY_Scene6; }
    
    std::string getSkybox() { return "newport_loft"; }

    virtual bool isEquirectangularMap() { return true; }
    
    virtual bool drawShadow() { return false; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,10.0f); }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void InitScene()
    {
        shader  = new Shader("pbr.vs","pbr.fs");
        ApplyCamera(shader);
        shader->attach("IRRADIANCE");
        shader->use();
        shader->setInt("albedoMap", 0);
        shader->setInt("normalMap", 1);
        shader->setInt("metallicMap", 2);
        shader->setInt("roughnessMap", 3);
        shader->setInt("aoMap", 4);
        shader->setInt("irradianceMap", 5);
        shader->setInt("prefilterMap", 6);
        shader->setInt("brdfLUT", 7);
        
        Texture("textures/pbr/rusted_iron/albedo", PNG, &albedo);
        Texture("textures/pbr/rusted_iron/normal", PNG, &normal);
        Texture("textures/pbr/rusted_iron/metallic",PNG, &metallic);
        Texture("textures/pbr/rusted_iron/roughness",PNG, &roughness);
        Texture("textures/pbr/rusted_iron/ao", PNG, &ao);
        InitBuffer();
        
        debugShader = new Shader("debug.vs", "debug.fs");
        InitQuad(quadVAO, quadVBO, debugShader);
    }
    
    void InitBuffer()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        bool oddRow = false;
        const uint X_SEGMENTS = 32;
        const uint Y_SEGMENTS = 32;
        std::vector<uint> indices;
        const float PI = 3.14159265359;
        for (uint y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (uint x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                
                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }
        for (int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();
        std::vector<float> data;
        for (int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }

    void DrawScene()
    {
        Scene::ClearScene();
        glBindVertexArray(vao);
        shader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedo);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallic);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughness);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ao);
        if(isEquirectangularMap())
        {
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->irradianceMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->prefilterMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->brdfLUTTexture);
        }
        
        mat4 model(1);
        model = translate(model, vec3(0));
        model = scale(model, vec3(1.6f));
        shader->setMat4("model", model);
        shader->setVec3("lightPositions[0]", vec3(0,0,4));
        shader->setVec3("lightColors[0]", vec3(40));
        shader->setVec3("lightPositions[1]", vec3(0,2,1));
        shader->setVec3("lightColors[1]", vec3(20));
        glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        RenderQuad(skybox->hdrTexture);
    }
 
    
private:
    Shader *shader;
    GLuint albedo, normal, metallic, roughness, ao;
    GLuint vao, vbo, ebo;
    size_t indexCount;

};


#endif /* scene6_h */
