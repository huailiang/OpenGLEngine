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
        BindVert(shader);
    }

    Material::~Material()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        foreach(it, textures) TexMgr::getInstance()->RemvTexture(it->second.textureID);
        textures.clear();
    }
    
    void Material::BindVert(Shader* sh)
    {
        if(sh)
        {
            shader = sh;
            mesh->BindVert(shader);
        }
    }
    
    void Material::Compile(Shader* sh)
    {
        if(sh)
        {
            sh->compile();
        }
        else
        {
            shader->compile();
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
            glDrawElements(DRAW_MODE, (GLsizei)mesh->num_indice, GL_UNSIGNED_SHORT, nullptr);
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
            glBindTexture(GL_TEXTURE_2D, it->second.textureID);
            i++;
        }
    }

    void Material::Draw(Shader* sh)
    {
        if(sh) shader=sh;
        error_stop(shader, "material shader can's be null");
        
        shader->use();
        DrawMesh();
        DrawTexture();
    }
    
    void Material::SetTexture(const char* name, const char* path, EXT ext)
    {
        if(strcmp(path, ""))
        {
            if(textures.find(name) == textures.end())
            {
                GLuint id;
                Texture tex(path,ext,&id);
                textures.insert(std::pair<const char*, Texture>(name,  tex));
            }
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

    
    ObjMaterial::ObjMaterial(MeshData* data, Shader* shader) : Material(data,shader)
    {
        diffuse_texture = 0;
        specul_texture = 0;
        ambient_texture = 0;
        normal_texure = 0;
    }
    
    
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
        mesh->ConfigAttribute(GL_STATIC_DRAW);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    
    void ObjMaterial::Draw(Shader* sh)
    {
        if(sh) this->shader= sh;
        error_stop(shader, "object material can't be null");
        
        mesh->BindVert(shader);
        shader->use();
        int i  = 0;
        if(diffuse_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            shader->setInt("texture_diffuse", i++);
            glBindTexture(GL_TEXTURE_2D, diffuse_texture);
        }
        if(normal_texure > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            shader->setInt("texture_normal", i++);
            glBindTexture(GL_TEXTURE_2D, normal_texure);
        }
        if(specul_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            shader->setInt("texture_specular", i++);
            glBindTexture(GL_TEXTURE_2D, specul_texture);
        }
        if(specul_texture > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            shader->setInt("texture_ambient", i++);
            glBindTexture(GL_TEXTURE_2D, ambient_texture);
        }
        DrawMesh();
    }
    
}
