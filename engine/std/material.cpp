//
//  xmesh.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/4/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "material.h"

namespace engine
{

    Material::Material(MeshData* data)
    {
        this->data = data;
    }


    Material::~Material()
    {
        delete data;
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void Material::SetupMesh()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        data->ConfigAttribute(GL_DYNAMIC_DRAW);
        glBindVertexArray(0);
    }


    void Material::DrawMesh()
    {
        glBindVertexArray(vao);
        glDrawElements(DRAW_MODE, (GLsizei)data->num_indice, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
    

    void Material::Draw(const Shader* shader)
    {
        shader->use();
        DrawMesh();
    }

    
    ObjMaterial::ObjMaterial(MeshData* data) : Material(data) { }
    
    
    ObjMaterial::~ObjMaterial()
    {
        glDeleteBuffers(1, &ebo);
        TexMgr::getInstance()->RemvTexture(diffuse_texture);
        TexMgr::getInstance()->RemvTexture(normal_texure);
        TexMgr::getInstance()->RemvTexture(ambient_texture);
        TexMgr::getInstance()->RemvTexture(specul_texture);
    }
    
    
    void ObjMaterial::SetupMesh()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        data->ConfigAttribute(GL_DYNAMIC_DRAW);
        glBindVertexArray(0);
    }
    
    
    void ObjMaterial::Draw(const Shader* shader)
    {
        shader->use();
        int i  = 0;
        if(diffuse_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glUniform1i(glGetUniformLocation(shader->ID, "texture_diffuse"), i++);
            glBindTexture(GL_TEXTURE_2D, diffuse_texture);
        }
        if(normal_texure > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glUniform1i(glGetUniformLocation(shader->ID, "texture_normal"), i++);
            glBindTexture(GL_TEXTURE_2D, normal_texure);
        }
        if(specul_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glUniform1i(glGetUniformLocation(shader->ID, "texture_specular"), i++);
            glBindTexture(GL_TEXTURE_2D, specul_texture);
        }
        if(specul_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glUniform1i(glGetUniformLocation(shader->ID, "texture_ambient"), i++);
            glBindTexture(GL_TEXTURE_2D, ambient_texture);
        }
        Material::Draw(shader);
    }

}
