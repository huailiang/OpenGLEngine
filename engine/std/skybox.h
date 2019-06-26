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
        
        Skybox(Camera* camera,std::string name,bool hdr=false);
        
        ~Skybox();
        
        void Draw();
        
        void Equirect2Cube();
        
    private:
        
        void init_tex(bool hdr);
        
        void init_buff();

    private:
        
        GLuint vao, vbo;
        std::vector<std::string> faces;
        GLuint cubemapTexture,hdrTexture;
        Shader* shader=nullptr;
        Shader* equirectangularToCubemapShader=nullptr;
        Camera* camera=nullptr;
        std::string name;
    };
    
}

#endif /* skybox_h */
