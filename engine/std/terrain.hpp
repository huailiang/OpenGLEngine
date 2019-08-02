//
//  terrain.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//


/*
 * 使用GPU Instance 画草
 */

#ifndef terrain_h
#define terrain_h

#include "texture.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "util.hpp"
#include "shadow.hpp"

namespace engine
{

    class Terrain
    {
    public:
        
        Terrain();
        
        ~Terrain();
        
        void initial();
        
        void InitInstance();
        
        void Draw(Camera* camera, glm::mat4 lightMatrix[], Light* light, Shadow* shadow);
        
        void DrawGrassInstance(Camera* camera);
        
    public:
        Shader* shader = nullptr;
        
    private:
        GLuint floor_vao, floor_vbo;
        GLuint grass_vao, grass_vbo, instVbo;
        GLuint floorTexture, grassTexture;
        std::vector<vec3> vegetation;
        uint grass_num;
        Shader* shader2 = nullptr;
        MeshData* gdata = nullptr;
    };
    
}

#endif /* terrain_h */
