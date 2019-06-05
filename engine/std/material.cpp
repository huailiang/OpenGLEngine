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
    SetupMesh();
}


Material::~Material()
{
    delete data;
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Material::SetupMesh()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data->num_vert * sizeof(Vertex), data->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->num_indice * sizeof(unsigned int), data->indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    glBindVertexArray(0);
}


void Material::DrawMesh()
{
    glBindVertexArray(vao);
    glDrawElements(DRAW_MODE, (GLsizei)data->num_indice, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Material::Draw(Shader* shader)
{
    int i  = 0;
    if(data->diffuse_texture > 0)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glUniform1i(glGetUniformLocation(shader->ID, "texture_diffuse"), i++);
        glBindTexture(GL_TEXTURE_2D, data->diffuse_texture);
    }
    if(data->normal_texure > 0)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glUniform1i(glGetUniformLocation(shader->ID, "texture_normal"), i++);
        glBindTexture(GL_TEXTURE_2D, data->normal_texure);
    }
    if(data ->specul_texture > 0)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glUniform1i(glGetUniformLocation(shader->ID, "texture_specular"), i++);
        glBindTexture(GL_TEXTURE_2D, data->specul_texture);
    }
    if(data->specul_texture > 0)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glUniform1i(glGetUniformLocation(shader->ID, "texture_ambient"), i++);
        glBindTexture(GL_TEXTURE_2D, data->ambient_texture);
    }
    
    DrawMesh();
}


}
