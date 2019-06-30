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
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
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
        shader->attach("IRRADIANCE");
        shader->use();
        ApplyCamera(shader);
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
        indexCount = InitSpere(&vao, &vbo, &ebo);
        
        debugShader = new Shader("debug.vs", "debug.fs");
        InitQuad(&quadVAO, &quadVBO, debugShader);
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
            glBindTexture(GL_TEXTURE_2D, skybox->brdfLUTTexture);
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
        
        RenderQuad(skybox->brdfLUTTexture);
    }
 
    
private:
    Shader *shader;
    GLuint albedo, normal, metallic, roughness, ao;
    GLuint vao, vbo, ebo;
    size_t indexCount;

};


#endif /* scene6_h */
