//
//  skybox.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/13/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef skybox_h
#define skybox_h

#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"

namespace engine
{


    class Skybox
    {
    public:
        
        Skybox(Camera* camera,std::string name);
        
        ~Skybox();
        
        void Draw();
        
    private:
        
        void init_tex();
        
        void init_buff();
        
        // loads a cubemap texture from 6 individual texture faces
        // order:
        // +X (right)
        // -X (left)
        // +Y (top)
        // -Y (bottom)
        // +Z (front)
        // -Z (back)
        unsigned int loadCubemap(std::vector<std::string> faces);
        
        
    private:
        
        unsigned int vao, vbo;
        std::vector<std::string> faces;
        unsigned int cubemapTexture;
        Shader* shader;
        Camera* camera;
        std::string name;
    };
    
}

#endif /* skybox_h */
