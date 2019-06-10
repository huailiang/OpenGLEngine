//
//  mesh.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/10/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "mesh.h"


namespace engine
{

    MeshData::~MeshData()
    {
        delete []indices;
        for(unsigned int i = 0; i < num_vert; i++)
            delete vertices[i];
        delete []vertices;
        
        indices = nullptr;
        vertices = nullptr;
    }
    
    void MeshData::ConfigAttribute(const GLenum usage)
    {
        if(num_indice > 0) glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indice * sizeof(unsigned int), indices, usage);
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
    
    BaseVert2* MeshData::GetBaseVertex2() const
    {
        BaseVert2* ptr = new BaseVert2[num_vert];
        if(type == 0x0012)
        {
            for(size_t i=0;i<num_vert;i++)
            {
                BaseVert2* v= (BaseVert2*)vertices[i];
                *(ptr+i) = *v;
            }
        }
        else
        {
            std::cerr<<"vert type error, 0x"<<hex<<type<<std::endl;
        }
        return ptr;
    }


    BaseVert3* MeshData::GetBaseVertex3() const
    {
        BaseVert3* ptr = new BaseVert3[num_vert];
        if(type == 0x0011)
        {
            for(size_t i=0;i<num_vert;i++)
            {
                BaseVert3* v= (BaseVert3*)vertices[i];
                *(ptr+i) = *v;
            }
        }
        else
        {
            std::cerr<<"BaseVert3 type error, 0x"<<hex<<type<<std::endl;
        }
        return ptr;
    }


    Vertex* MeshData::GetVertex() const
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
            std::cerr<<"Vertex type error, 0x"<<hex<<type<<std::endl;
        }
        return ptr;
    }

    ColorVertex* MeshData::GetColorVertex() const
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
            std::cerr<<"ColorVertex type error, 0x"<<hex<<type<<std::endl;
        }
        return ptr;
    }

    CompxVertex* MeshData::GetCompxVertex() const
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
            std::cerr<<"CompxVertex type error, 0x"<<hex<<type<<std::endl;
        }
        return ptr;
    }

    bool MeshData::hasPos() const
    {
        return (type & Vt_Pos3) > 0 || (type & Vt_Pos2) > 0;
    }
    
    bool MeshData::hasPos2() const
    {
        return (type & Vt_Pos2) > 0;
    }
    
    bool MeshData::hasPos3() const
    {
        return (type & Vt_Pos3) > 0;
    }

    bool MeshData::hasUV() const
    {
        return (type & Vt_UV) > 0;
    }

    bool MeshData::hasNormal() const
    {
        return (type & Vt_Normal) > 0;
    }

    bool MeshData::hasColor() const
    {
        return (type & Vt_Color) > 0;
    }


}
