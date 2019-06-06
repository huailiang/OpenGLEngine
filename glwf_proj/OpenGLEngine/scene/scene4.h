//
//  scene4.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/26/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene4_h
#define scene4_h

#include "scene.h"
#include "profile.h"

class Scene4 : public Scene
{
    
public:
    ~Scene4()
    {
        SAFE_DELETE(shader);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }
    
    int getType() { return TY_Scene4; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,8.0f); }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void InitScene()
    {
        shader  = new Shader("pbr.vs","pbr.fs");
        ApplyCamera(shader);
        
        shader->use();
        shader->setInt("albedoMap", 0);
        shader->setInt("normalMap", 1);
        shader->setInt("metallicMap", 2);
        shader->setInt("roughnessMap", 3);
        shader->setInt("aoMap", 4);
        
        Texture("resources/textures/pbr/rusted_iron/albedo.png", &albedo);
        Texture("resources/textures/pbr/rusted_iron/normal.png", &normal);
        Texture("resources/textures/pbr/rusted_iron/metallic.png", &metallic);
        Texture("resources/textures/pbr/rusted_iron/roughness.png", &roughness);
        Texture("resources/textures/pbr/rusted_iron/ao.png", &ao);
        
        InitBuffer();
    }
    
    void InitBuffer()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
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
        bool oddRow = false;
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
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

        mat4 model(1);
        for (int row = 0; row < nGrid; ++row)
        {
            for (int col = 0; col < nGrid; ++col)
            {
                model = glm::mat4(1.0f);
                model = translate(model, vec3((float)(col - (nGrid / 2)) * spacing, (float)(row - (nGrid / 2)) * spacing, 0));
                shader->setMat4("model", model);
                shader->setVec3("lightPositions[0]", vec3(0,0,4));
                shader->setVec3("lightColors[0]", vec3(40));
                shader->setVec3("lightPositions[1]", vec3(0,2,1));
                shader->setVec3("lightColors[1]", vec3(20));
                glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)indexCount, GL_UNSIGNED_INT, 0);
            }
        }
        glBindVertexArray(0);
    }
    
    
private:
    Shader *shader;
    GLuint vao, vbo, ebo;
    GLuint albedo, normal, metallic, roughness, ao;
    int nGrid = 6;
    float spacing = 2.5;
    size_t indexCount;
};


#endif /* scene4_h */
