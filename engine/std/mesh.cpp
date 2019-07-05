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
    
    #define GET_VERTEX(T) \
    T *p = new T[num_vert]; \
    for(size_t i=0;i<num_vert;i++) {  \
        T* v = (T*)vertices[i]; \
        *(p+i) = *v;    \
    }
    
    
    #define VERT_ATTRIB_PTR(idx, size, T, offset)   \
    glEnableVertexAttribArray(idx); \
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)(offset * sizeof(float)));

    
    #define MESH_VERT_CONFIG(config, num, T)   \
    glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(T), p, usage);  \
    int offset = 0;    \
    for (int i=0; i<num; i++) { \
        VERT_ATTRIB_PTR(i, *(config+i), T, offset);    \
        offset+= *(config+i);   \
    }

    
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
        Vert* p = nullptr;
        if(type == 0x0111 || type == 0x1011)
        {
            GET_VERTEX(Vertex);
            int config[] = { LEN_Pos3, LEN_UV, LEN_Normal };
            MESH_VERT_CONFIG(config, 3, Vertex);
        }
        if(type == 0x0101)
        {
            GET_VERTEX(NormalVert);
            int config[] = { LEN_Pos3, LEN_Normal };
            MESH_VERT_CONFIG(config, 2, NormalVert);
        }
        else if(type == 0x1111)
        {
            GET_VERTEX(CompxVertex);
            int config[] = { LEN_Pos3, LEN_UV, LEN_Normal, LEN_Color };
            MESH_VERT_CONFIG(config, 4, CompxVertex);
        }
        else if (type == 0x0011)
        {
            GET_VERTEX(BaseVert3);
            int config[] = { LEN_Pos3, LEN_UV };
            MESH_VERT_CONFIG(config, 2, BaseVert3);
        }
        else if (type == 0x0012)
        {
            GET_VERTEX(BaseVert2);
            int config[] = { LEN_Pos2, LEN_UV };
            MESH_VERT_CONFIG(config, 2, BaseVert2);
        }
        else if(type == 0x2111)
        {
            GET_VERTEX(SkeletonVertex);
            int config[] = { LEN_Pos3, LEN_UV, LEN_Normal, LEN_Skin };
            MESH_VERT_CONFIG(config, 4, SkeletonVertex);
        }
        else
        {
            std::cerr<<"not support vertex attribute: 0x"<<hex<<type<<std::endl;
        }
        if(p) delete [] p;
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
    
    bool MeshData::Bind(Shader* shader)
    {
        if(shader && !shader->compiled)
        {
            std::stringstream stream;
            stream.width(4);
            stream.fill('0');
            stream<<std::hex<<type;
            std::string macro = "0x"+stream.str();
            shader->attach("VERT_TYPE", macro.c_str());
            return true;
        }
        return false;
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
