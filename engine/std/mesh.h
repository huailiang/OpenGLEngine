//
//  mesh.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/9/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef mesh_h
#define mesh_h


#include "texmgr.h"
#include "shader.h"

namespace engine
{
    
    #define Vt_Pos3    0x0001
    #define Vt_Pos2    0x0002
    #define Vt_UV      0x0010
    #define Vt_UV2     0x0020
    #define Vt_UV3     0x0040
    #define Vt_UV4     0x0040
    #define Vt_Normal  0x0100
    #define Vt_TAN     0x0200
    #define Vt_BIT     0x0400
    #define Vt_Color   0x1000
    #define Vt_Skin    0x2000
    
    #define LEN_Pos3    3
    #define LEN_Pos2    2
    #define LEN_UV      2
    #define LEN_UV2     2
    #define LEN_UV3     2
    #define LEN_UV4     2
    #define LEN_Normal  3
    #define LEN_TAN     3
    #define LEN_BIT     3
    #define LEN_Color   3
    #define LEN_Skin    4

    
    typedef unsigned int VertType;
    
    #define VIRTUAL_FUNC_CNT 1
    
    struct Vert { };
    
    struct BaseVert2 : Vert
    {
        glm::vec2 Position;
        glm::vec2 TexCoords;
    };
    
    struct BaseVert3 : Vert
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
    };
    
    struct NormalVert:Vert
    {
        glm::vec3 Position;
        glm::vec3 Normal;
    };
    
    struct Vertex : Vert
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Normal;
    };
    
    struct SkinVertex : Vertex
    {
        float weight[3];
        int boneindx[3];
        int bonecount;
        
        SkinVertex()
        {
            bonecount=0;
            loop(3) weight[i] = 0;
            loop(3) boneindx[i] = 0;
        }
    };
    
    struct SkeletonVertex : Vertex
    {
        glm::vec4 Bone;
    };
    
    
    struct ColorVertex : Vert
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Color;
    };
    
    struct CompxVertex : Vertex
    {
        glm::vec3 Color;
    };
    
    struct TangVertex : Vertex
    {
        glm::vec3 Tangent;
    };
    
    
    struct MeshData
    {
        uint num_indice;
        unsigned short *indices= nullptr;
        uint num_vert;
        Vert* vertices = nullptr;
        VertType type;
        
        ~MeshData();
        
        void ConfigAttribute(const GLenum usage = GL_STATIC_DRAW);
        
        bool hasPos() const;

        bool hasPos3() const;

        bool hasPos2() const;

        bool hasUV() const;
        
        bool hasNormal() const;
        
        bool hasColor() const;
        
        bool BindVert(Shader* shader);
        
        /*
         * you need to delete[] after the function be called
         */
        TangVertex* RecalcuteTangent();
        
    };

}

#endif /* mesh_h */
