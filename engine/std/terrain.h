//
//  terrain.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef terrain_h
#define terrain_h

#include "common.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "light.h"


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
        
        void Draw(Camera* camera, glm::mat4 lightMatrix, Light* light, unsigned int depthMap);
        
        void DrawSample(Camera* camera);
        
        void DrawGrassDirect(Camera* camera);
        
        void DrawGrassInstance(Camera* camera);
        
    private:
        
        unsigned int floor_vao, floor_vbo;
        unsigned int grass_vao, grass_vbo, instVbo;
        unsigned int floorTexture, grassTexture;
        std::vector<vec3> vegetation;
        unsigned int grass_num;
        Shader* shader;
        Shader* shader2;
    };
    
}

#endif /* terrain_h */
