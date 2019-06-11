//
//  MeshData.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/9/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef MeshData_h
#define MeshData_h


#include "texmgr.h"

namespace engine
{
    
    #define Vt_Pos3    0x0001
    #define Vt_Pos2    0x0002
    #define Vt_UV      0x0010
    #define Vt_UV2     0x0020
    #define Vt_UV3     0x0040
    #define Vt_Normal  0x0100
    #define Vt_TAN     0x0200
    #define Vt_BIT     0x0400
    #define Vt_Color   0x1000
    
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
        unsigned int num_indice;
        unsigned int *indices;
        unsigned int num_vert;
        Vert** vertices;
        VertType type;
        
        ~MeshData();
        
        void ConfigAttribute(const GLenum usage = GL_STATIC_DRAW);
        
        BaseVert2* GetBaseVertex2() const;
        
        BaseVert3* GetBaseVertex3() const;
        
        Vertex* GetVertex() const;
        
        ColorVertex* GetColorVertex() const;
        
        CompxVertex* GetCompxVertex() const;
        
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

#endif /* MeshData_h */
