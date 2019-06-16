//
//  mesh.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/10/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "mesh.h"
#include "util.h"

namespace engine
{

    MeshData::~MeshData()
    {
        delete []indices;
        for(uint i = 0; i < num_vert; i++)
            delete vertices[i];
        delete []vertices;
        
        indices = nullptr;
        vertices = nullptr;
    }
    
    void MeshData::ConfigAttribute(const GLenum usage)
    {
        if(num_indice > 0) glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indice * sizeof(uint), indices, usage);
     
        
        if(type == 0x0111 || type == 0x1011)
        {
            Vert* p = GetVertex();
            glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(Vertex), p, usage);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(float))); //normal is equal color
            delete [] p;
        }
        else if(type == 0x1111)
        {
            CompxVertex* p = GetCompxVertex();
            glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(CompxVertex), p, usage);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CompxVertex), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CompxVertex), (void*)offsetof(CompxVertex, TexCoords));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CompxVertex), (void*)offsetof(CompxVertex, Normal));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CompxVertex), (void*)offsetof(CompxVertex, Color));
            delete [] p;
        }
        else if (type == 0x0011)
        {
            BaseVert3* p = GetBaseVertex3();
            glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(BaseVert3), p, usage);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BaseVert3), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVert3), (void*)offsetof(BaseVert3, TexCoords));
            delete [] p;
        }
        else if (type == 0x0012)
        {
            BaseVert2* p = GetBaseVertex2();
            glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(BaseVert2), p, usage);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVert2), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVert2), (void*)offsetof(BaseVert2, TexCoords));
            delete [] p;
        }
        else if(type == 0x2111)
        {
            SkeletonVertex* p = GetSkinVertex();
            glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(SkeletonVertex), p, usage);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletonVertex), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SkeletonVertex), (void*)(3*sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletonVertex), (void*)(5*sizeof(float)));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletonVertex), (void*)(8*sizeof(float)));
            glEnableVertexAttribArray(4);
            glVertexAttribIPointer(4, 3, GL_INT, sizeof(SkeletonVertex), (void*)(11*sizeof(float)));
            delete [] p;
        }
        else
        {
            std::cerr<<"not support vertex attribute: 0x"<<hex<<type<<std::endl;
        }
    }
        
    BaseVert2* MeshData::GetBaseVertex2() const
    {
        BaseVert2* ptr = nullptr;
        if(type == 0x0012)
        {
            ptr = new BaseVert2[num_vert];
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
        BaseVert3* ptr = nullptr;
        if(type == 0x0011)
        {
            ptr = new BaseVert3[num_vert];
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
        Vertex* ptr = nullptr;
        if(type == 0x0111)
        {
            ptr = new Vertex[num_vert];
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
        ColorVertex* ptr = nullptr;
        if(type == 0x1011)
        {
            ptr = new ColorVertex[num_vert];
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
        CompxVertex* ptr = nullptr;
        if(type == 0x1111)
        {
            ptr = new CompxVertex[num_vert];
            for(uint i=0;i<num_vert;i++)
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
    
    SkeletonVertex* MeshData::GetSkinVertex() const
    {
        SkeletonVertex* ptr = nullptr;
        if(type == 0x2111)
        {
            ptr = new SkeletonVertex[num_vert];
            for(uint i=0;i<num_vert;i++)
            {
                SkeletonVertex* v= (SkeletonVertex*)vertices[i];
                *(ptr+i) = *v;
            }
        }
        else
        {
            std::cerr<<"SkinVertex type error, 0x"<<hex<<type<<std::endl;
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
    
    /*
     * you need to delete[] after the function be called
     */
    TangVertex* MeshData::RecalcuteTangent()
    {
        TangVertex* ptr = new TangVertex[num_vert];
        for (size_t i=0; i<num_indice; i+=3)
        {
            uint idx0 = indices[i+0];
            uint idx1 = indices[i+1];
            uint idx2 = indices[i+2];
            Vert* v0 = vertices[idx0];
            Vert* v1 = vertices[idx1];
            Vert* v2 = vertices[idx2];
            TangVertex* tv0 = ptr + idx0;
            TangVertex* tv1 = ptr + idx0;
            TangVertex* tv2 = ptr + idx0;
            
            glm::vec3 *tan = nullptr, *bit = nullptr;
            if(type == 0x0111)
            {
                caltangent((Vertex*)v0, (Vertex*)v1, (Vertex*)v2,tan,bit);
                ConvertVertex((Vertex*)v0, tv0);
                ConvertVertex((Vertex*)v1, tv1);
                ConvertVertex((Vertex*)v2, tv2);
            }
            else if(type == 0x1111)
            {
                caltangent((CompxVertex*)v0,(CompxVertex*)v1,(CompxVertex*)v2,tan,bit);
                ConvertVertex((CompxVertex*)v0, tv0);
                ConvertVertex((CompxVertex*)v1, tv1);
                ConvertVertex((CompxVertex*)v2, tv2);
            }
            else
            {
                std::cerr<<"MESH RecalcuteTangent, not support tangent for format: 0x"<<hex<<type<<std::endl;
            }
            tv0->Tangent = *tan;
            tv1->Tangent = *tan;
            tv2->Tangent = *tan;
        }
        return ptr;
    }
    
}
