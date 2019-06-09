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
        
        Material(MeshData* data);
        
        virtual ~Material();
        
        void SetupMesh();
        
        virtual void Draw(Shader* shader);
        
        void DrawMesh();
        
    public:
        
        MeshData* data;
        
    protected:
        
        GLuint ebo, vao, vbo;
        
    };
    
    
    class ObjMaterial : public Material
    {
    public:
        
        ObjMaterial(MeshData* data);
        
        virtual ~ObjMaterial();
        
        virtual void Draw(Shader* shader);
        
    public:
        GLuint diffuse_texture;
        GLuint normal_texure;
        GLuint ambient_texture;
        GLuint specul_texture;
        
    };
    
}

#endif /* material_h */
