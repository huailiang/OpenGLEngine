//
//  xmesh.hpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/4/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef material_h
#define material_h

#include "mesh.h"
#include "shader.h"
#include "texmgr.h"

#define TEXTURE_NUM 4

namespace engine
{

    class Material
    {
        
    public:
        
        Material(MeshData* data, Shader* shader = nullptr);
        
        virtual ~Material();
        
        virtual void SetupMesh();
        
        virtual void Draw(Shader* shader = nullptr);
        
        void AttachShader(Shader* shader);
        
        void DrawMesh();
        
        void SetTexture(const char* name,const char* path, EXT ext);
        
        void SetInt(const char* name, int value);
        
        void SetFloat(const char* name, float value);
        
        void SetVec2(const char* name, glm::vec2 v2);
        
        void SetVec3(const char* name, glm::vec3 v3);
        
        void SetVec4(const char* name, glm::vec4 v4);
        
        void SetMat3(const char* name, glm::mat3 m3);
        
        void SetMat4(const char* name, glm::mat4 m4);
        
    private:
        
        void DrawTexture();
        
    public:
        MeshData* mesh = nullptr;
        Shader* shader = nullptr;
        
    protected:
        GLuint vao, vbo;
        vector<const char*> texpaths;
        map<const char*, GLuint> textures;
        
    };
    
    //.obj model material
    class ObjMaterial : public Material
    {
        
    public:
        
        ObjMaterial(MeshData* data, Shader* shader = nullptr);
        
        virtual ~ObjMaterial();
        
        virtual void SetupMesh();
        
        virtual void Draw(Shader* shader);
        
    public:
        GLuint diffuse_texture;
        GLuint normal_texure;
        GLuint ambient_texture;
        GLuint specul_texture;
        
    protected:
        GLuint ebo;
        
    };
    
    
}

#endif /* material_h */
