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

    Material::Material(MeshData* data, Shader* shader)
    {
        this->mesh = data;
        vao = 0;
        vbo = 0;
        AttachShader(shader);
    }

    Material::~Material()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        foreach(it, textures) TexMgr::getInstance()->RemvTexture(it->second);
        texpaths.clear();
        textures.clear();
    }
    
    void Material::AttachShader(Shader* shader)
    {
        if(shader)
        {
            this->shader = shader;
            mesh->Bind(shader);
        }
    }

    void Material::SetupMesh()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        mesh->ConfigAttribute(GL_STATIC_DRAW);
        glBindVertexArray(0);
    }

    void Material::DrawMesh()
    {
        if(vao > 0)
        {
            glBindVertexArray(vao);
            glDrawElements(DRAW_MODE, (GLsizei)mesh->num_indice, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }
    
    void Material::DrawTexture()
    {
        int i=0;
        foreach(it, textures)
        {
            glActiveTexture(GL_TEXTURE0+i);
            shader->setInt(it->first, i);
            glBindTexture(GL_TEXTURE_2D, it->second);
            i++;
        }
    }

    void Material::Draw(Shader* shader)
    {
        if(shader!=nullptr)
            this->shader = shader;
        
        this->shader->use();
        DrawMesh();
        DrawTexture();
    }
    
    void Material::SetTexture(const char* name, const char* path, EXT ext)
    {
        bool empty = strcmp(path, "") == 0;
        if(!empty && std::find(texpaths.begin(), texpaths.end(), path) ==  texpaths.end())
        {
            GLuint id;
            Texture(path,ext,&id);
            texpaths.push_back(path);
            textures.insert(std::pair<const char*, GLuint>(name,  id));
        }
    }
    
    void Material::SetInt(const char* name, int value)
    {
        if(shader) shader->setInt(name, value);
    }
    
    void Material::SetFloat(const char* name, float value)
    {
        if(shader) shader->setFloat(name, value);
    }
    
    void Material::SetVec2(const char* name, glm::vec2 v2)
    {
        if(shader) shader->setVec2(name, v2);
    }
    
    void Material::SetVec3(const char* name, glm::vec3 v3)
    {
        if(shader) shader->setVec3(name, v3);
    }
    
    void Material::SetVec4(const char* name, glm::vec4 v4)
    {
        if(shader) shader->setVec4(name, v4);
    }
    
    void Material::SetMat3(const char *name, glm::mat3 m3)
    {
        if(shader) shader->setMat3(name, m3);
    }
    
    void Material::SetMat4(const char* name, glm::mat4 m4)
    {
        if(shader) shader->setMat4(name, m4);
    }

    
    ObjMaterial::ObjMaterial(MeshData* data, Shader* shader) : Material(data,shader) { }
    
    
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
        mesh->ConfigAttribute(GL_DYNAMIC_DRAW);
        glBindVertexArray(0);
    }
    
    
    void ObjMaterial::Draw(Shader* shader)
    {
        if (shader != nullptr)
            this->shader = shader;
        
        mesh->Bind(this->shader);
        this->shader->use();
        int i  = 0;
        if(diffuse_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            this->shader->setInt("texture_diffuse", i++);
            glBindTexture(GL_TEXTURE_2D, diffuse_texture);
        }
        if(normal_texure > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            this->shader->setInt("texture_normal", i++);
            glBindTexture(GL_TEXTURE_2D, normal_texure);
        }
        if(specul_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            this->shader->setInt("texture_specular", i++);
            glBindTexture(GL_TEXTURE_2D, specul_texture);
        }
        if(specul_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            this->shader->setInt("texture_ambient", i++);
            glBindTexture(GL_TEXTURE_2D, ambient_texture);
        }
        DrawMesh();
    }
    
}
