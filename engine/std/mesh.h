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
    
    typedef unsigned int VertType;
    
    struct Vert { };
    
    // type = 0x0012
    struct BaseVert2 : Vert
    {
        glm::vec2 Position;
        glm::vec2 TexCoords;
    };
    
    // type = 0x0011
    struct BaseVert3 : Vert
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
    };
    
    // type = 0x0111
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
    
    //type = 0x2111
    struct SkeletonVertex : Vert
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Normal;
        glm::vec3 weight;
        glm::ivec3 boneindx;
    };
    
    
    // type = 0x1011
    struct ColorVertex : Vert
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Color;
    };
    
    // type = 0x1111
    struct CompxVertex : Vert
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Normal;
        glm::vec3 Color;
    };
    
    // type = 0x0311
    struct TangVertex : Vert
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Normal;
        glm::vec3 Tangent;
    };

    struct MeshData
    {
        uint num_indice;
        uint *indices;
        uint num_vert;
        Vert** vertices;
        VertType type;
        
        ~MeshData();
        
        void ConfigAttribute(const GLenum usage = GL_STATIC_DRAW);
        
        BaseVert2* GetBaseVertex2() const;
        
        BaseVert3* GetBaseVertex3() const;
        
        Vertex* GetVertex() const;
        
        ColorVertex* GetColorVertex() const;
        
        CompxVertex* GetCompxVertex() const;
        
        SkeletonVertex* GetSkinVertex() const;
        
        bool hasPos() const;

        bool hasPos3() const;

        bool hasPos2() const;

        bool hasUV() const;
        
        bool hasNormal() const;
        
        bool hasColor() const;
        
        /*
         * you need to delete[] after the function be called
         */
        TangVertex* RecalcuteTangent();
        
    };

}

#endif /* mesh_h */
