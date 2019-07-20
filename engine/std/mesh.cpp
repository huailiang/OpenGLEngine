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
    
    #define VERT_ATTRIB_PTR(idx, size, T, offset)   \
    glEnableVertexAttribArray(idx); \
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)(offset * sizeof(float)));
    
    
    #define MESH_VERT_CONFIG(config, num, T)   \
    glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(T), vertices, usage);  \
    int offset = 0;    \
    for (int i=0; i<num; i++) { \
        VERT_ATTRIB_PTR(i, *(config+i), T, offset);    \
        offset+= *(config+i);   \
    }

    
    MeshData::~MeshData()
    {
        delete [] indices;
        delete [] vertices;
        indices = nullptr;
        vertices = nullptr;
        num_vert = 0;
        num_indice = 0;
    }
    
    
    void MeshData::ConfigAttribute(const GLenum usage)
    {
        if(num_indice) glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indice * sizeof(unsigned short), indices, usage);
        if(type == 0x0111 || type == 0x1011)
        {
            int config[] = { LEN_Pos3, LEN_UV, LEN_Normal };
            MESH_VERT_CONFIG(config, 3, Vertex);
        }
        else if(type == 0x0101)
        {
            int config[] = { LEN_Pos3, LEN_Normal };
            MESH_VERT_CONFIG(config, 2, NormalVert);
        }
        else if(type == 0x1111)
        {
            int config[] = { LEN_Pos3, LEN_UV, LEN_Normal, LEN_Color };
            MESH_VERT_CONFIG(config, 4, CompxVertex);
        }
        else if (type == 0x0011)
        {
            int config[] = { LEN_Pos3, LEN_UV };
            MESH_VERT_CONFIG(config, 2, BaseVert3);
        }
        else if (type == 0x0012)
        {
            int config[] = { LEN_Pos2, LEN_UV };
            MESH_VERT_CONFIG(config, 2, BaseVert2);
        }
        else if(type == 0x2111)
        {
            int config[] = { LEN_Pos3, LEN_UV, LEN_Normal, LEN_Skin };
            MESH_VERT_CONFIG(config, 4, SkeletonVertex);
        }
        else
        {
            std::cerr<<"not support vertex attribute: 0x"<<hex<<type<<std::endl;
        }
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
    
    bool MeshData::BindVert(Shader* shader)
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
            TangVertex* tv0 = ptr + idx0;
            TangVertex* tv1 = ptr + idx0;
            TangVertex* tv2 = ptr + idx0;
            
            glm::vec3 *tan = nullptr, *bit = nullptr;
            if(type == 0x0111)
            {
                caltangent((Vertex*)vertices + idx0, (Vertex*)vertices + idx1, (Vertex*)vertices + idx2,tan,bit);
                ConvertVertex((Vertex*)vertices + idx0, tv0);
                ConvertVertex((Vertex*)vertices + idx1, tv1);
                ConvertVertex((Vertex*)vertices + idx2, tv2);
            }
            else if(type == 0x1111)
            {
                caltangent((CompxVertex*)vertices + idx0,(CompxVertex*)vertices + idx1,(CompxVertex*)vertices + idx2,tan,bit);
                ConvertVertex((CompxVertex*)vertices + idx0, tv0);
                ConvertVertex((CompxVertex*)vertices + idx1, tv1);
                ConvertVertex((CompxVertex*)vertices + idx2, tv2);
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
