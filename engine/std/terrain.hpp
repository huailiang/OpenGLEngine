//
//  terrain.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef terrain_h
#define terrain_h

#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "util.hpp"

#define _Instance_

namespace engine
{

    class Terrain
    {
    public:
        
        Terrain();
        
        ~Terrain();
        
        void initial();
        
        void InitInstance();

        void DrawShadow(const Shader *shader);
        
        void Draw(Camera* camera, glm::mat4 lightMatrix, Light* light, GLuint depthMap);
        
        void DrawSample(Camera* camera);
        
        void DrawGrassDirect(Camera* camera);
        
        void DrawGrassInstance(Camera* camera);
        
    private:
        
        GLuint floor_vao, floor_vbo;
        GLuint grass_vao, grass_vbo, instVbo;
        GLuint floorTexture, grassTexture;
        std::vector<vec3> vegetation;
        uint grass_num;
        Shader* shader = nullptr;
        Shader* shader2 = nullptr;
        MeshData* gdata = nullptr;
    };
    
}

#endif /* terrain_h */
