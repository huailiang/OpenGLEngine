//
//  MeshData.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/9/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef MeshData_h
#define MeshData_h

#include "../common.h"
#include "texmgr.h"

namespace engine
{
    
    #define Vt_Pos     0x0001
    #define Vt_UV      0x0010
    #define Vt_Normal  0x0100
    #define Vt_Color   0x1000
    
    typedef unsigned int VertType;
    
    struct Vert { };
    
    struct Vertex : Vert
    {
        glm::vec3 Position;
        // uv
        glm::vec2 TexCoords;
        // normal
        glm::vec3 Normal;
    };
    
    struct ColorVertex : Vert
    {
        glm::vec3 Position;
        // uv
        glm::vec2 TexCoords;
        // color
        glm::vec3 Color;
    };
    
    struct CompxVertex:Vert
    {
        glm::vec3 Position;
        // uv
        glm::vec2 TexCoords;
        // normal
        glm::vec3 Normal;
        // color
        glm::vec3 Color;
    };

    struct MeshData
    {
        unsigned int num_indice;
        unsigned int *indices;
        unsigned int num_vert;
        Vertex* vertices;
        VertType type;
        
        ~MeshData()
        {
            delete []indices;
            delete []vertices;
            indices = NULL;
            vertices = NULL;
        }
        
        bool hasPos()
        {
            return (type & Vt_Pos) > 0;
        }
        
        bool hasUV()
        {
            return (type & Vt_UV) > 0;
        }
        
        bool hasNormal()
        {
            return (type & Vt_Normal) > 0;
        }
        
        bool hasColor()
        {
            return (type & Vt_Color) > 0;
        }
        
    };


}

#endif /* MeshData_h */
