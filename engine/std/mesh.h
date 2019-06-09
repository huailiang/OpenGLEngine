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
    #define Vt_TAN     0x0020
    #define Vt_BIT     0x0030
    #define Vt_Normal  0x0100
    #define Vt_Color   0x1000
    
    typedef unsigned int VertType;
    
    struct Vert { };
    
    struct BaseVert2 : Vert
    {
        glm::vec2 Position;
        glm::vec2 TexCoords;
    };
    
    struct BaseVert3 : Vert
    {
        glm::vec3 Position;
        // uv
        glm::vec2 TexCoords;
    };
    
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
            for(unsigned int i = 0; i < num_vert; i++)
                delete vertices[i];
            delete []vertices;
            
            indices = nullptr;
            vertices = nullptr;
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
            else if (type == 0x0011)
            {
                glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(BaseVert3), GetBaseVertex3(), usage);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BaseVert3), nullptr);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVert3), (void*)offsetof(BaseVert3, TexCoords));
            }
            else if (type == 0x0012)
            {
                glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(BaseVert2), GetBaseVertex2(), usage);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVert2), nullptr);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVert2), (void*)offsetof(BaseVert2, TexCoords));
            }
            else
            {
                std::cerr<<"not support vertex attribute: 0x"<<hex<<type<<std::endl;
            }
        }
        
        BaseVert2* GetBaseVertex2()
        {
            BaseVert2* ptr = new BaseVert2[num_vert];
            if(type == 0x0111)
            {
                for(size_t i=0;i<num_vert;i++)
                {
                    BaseVert2* v= (BaseVert2*)vertices[i];
                    *(ptr+i) = *v;
                }
            }
            else
            {
                std::cerr<<"vert type error"<<std::endl;
            }
            return ptr;
        }

        
        BaseVert3* GetBaseVertex3()
        {
            BaseVert3* ptr = new BaseVert3[num_vert];
            if(type == 0x0111)
            {
                for(size_t i=0;i<num_vert;i++)
                {
                    BaseVert3* v= (BaseVert3*)vertices[i];
                    *(ptr+i) = *v;
                }
            }
            else
            {
                std::cerr<<"vert type error"<<std::endl;
            }
            return ptr;
        }
        
        
        Vertex* GetVertex()
        {
            Vertex* ptr = new Vertex[num_vert];
            if(type == 0x0111)
            {
                for(size_t i=0;i<num_vert;i++)
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
                for(size_t i=0;i<num_vert;i++)
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
                for(unsigned int i=0;i<num_vert;i++)
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
            return (type & Vt_Pos3) > 0 || (type & Vt_Pos2) > 0;
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
