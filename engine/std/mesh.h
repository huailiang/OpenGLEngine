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
    
    struct CompxVertex : Vert
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
        Vert** vertices;
        VertType type;
        
        ~MeshData()
        {
            delete []indices;
            for(int i = 0; i < num_vert; i++)
                delete vertices[i];
            delete []vertices;
            
            indices = NULL;
            vertices = NULL;
        }
        
        void ConfigAttribute( GLenum usage = GL_STATIC_DRAW)
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indice * sizeof(unsigned int), indices, usage);
            if(type == 0x0111 || type == 0x1011)
            {
                glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(Vertex), GetVertex(), usage);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); //normal is equal color
            }
            else if(type == 0x1111)
            {
                glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(CompxVertex), GetCompxVertex(), usage);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CompxVertex), nullptr);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CompxVertex), (void*)offsetof(CompxVertex, TexCoords));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CompxVertex), (void*)offsetof(CompxVertex, Normal));
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CompxVertex), (void*)offsetof(CompxVertex, Color));
            }
        }
        
        
        Vertex* GetVertex()
        {
            Vertex* ptr = new Vertex[num_vert];
            if(type == 0x0111)
            {
                for(int i=0;i<num_vert;i++)
                {
                    Vertex* v= (Vertex*)vertices[i];
                    *(ptr+i) = *v;
                }
            }
            else
            {
                std::cerr<<"vert type error"<<std::endl;
            }
            return ptr;
        }
        
        ColorVertex* GetColorVertex()
        {
            ColorVertex* ptr = new ColorVertex[num_vert];
            if(type == 0x1011)
            {
                for(int i=0;i<num_vert;i++)
                {
                    ColorVertex* v= (ColorVertex*)vertices[i];
                    *(ptr+i) = *v;
                }
            }
            else
            {
                std::cerr<<"vert type error"<<std::endl;
            }
            return ptr;
        }
        
        CompxVertex* GetCompxVertex()
        {
            CompxVertex* ptr = new CompxVertex[num_vert];
            if(type == 0x1111)
            {
                for(int i=0;i<num_vert;i++)
                {
                    CompxVertex* v= (CompxVertex*)vertices[i];
                    *(ptr+i) = *v;
                }
            }
            else
            {
                std::cerr<<"vert type error"<<std::endl;
            }
            return ptr;
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
