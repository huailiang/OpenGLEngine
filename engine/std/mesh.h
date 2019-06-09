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


    struct Vertex
    {
        glm::vec3 Position;
        //uv
        glm::vec2 TexCoords;
        //normal
        glm::vec3 Normal;
    };

    struct MeshData
    {
        unsigned int num_indice;
        unsigned int *indices;
        unsigned int num_vert;
        Vertex* vertices;
        
//        GLuint diffuse_texture;
//        GLuint normal_texure;
//        GLuint ambient_texture;
//        GLuint specul_texture;
        
        
        ~MeshData()
        {
            delete []indices;
            delete []vertices;
            indices = NULL;
            vertices = NULL;
//            TexMgr::getInstance()->RemvTexture(diffuse_texture);
//            TexMgr::getInstance()->RemvTexture(normal_texure);
//            TexMgr::getInstance()->RemvTexture(ambient_texture);
//            TexMgr::getInstance()->RemvTexture(specul_texture);
        }
    };


}

#endif /* MeshData_h */
