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

class Scene4 : public Scene
{
    
public:
    
    ~Scene4()
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
    
    int getType() { return TY_Scene4; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,12.0f); }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void InitScene()
    {
        shader  = new Shader("pbr.vs","pbr.fs");
        
        shader->use();
        shader->setInt("albedoMap", 0);
        shader->setInt("normalMap", 1);
        shader->setInt("metallicMap", 2);
        shader->setInt("roughnessMap", 3);
        shader->setInt("aoMap", 4);
        ApplyCamera(shader);
        
        Texture("textures/pbr/rusted_iron/albedo", _PNG, &albedo);
        Texture("textures/pbr/rusted_iron/normal", _PNG, &normal);
        Texture("textures/pbr/rusted_iron/metallic",_PNG, &metallic);
        Texture("textures/pbr/rusted_iron/roughness",_PNG, &roughness);
        Texture("textures/pbr/rusted_iron/ao", _PNG, &ao);
        indexCount = InitSpere(&vao, &vbo, &ebo);
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
    Shader *shader = nullptr;
    GLuint vao, vbo, ebo;
    GLuint albedo, normal, metallic, roughness, ao;
    int nGrid = 3;
    float spacing = 3;
    size_t indexCount;
};


#endif /* scene4_h */
