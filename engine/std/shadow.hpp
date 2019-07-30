//
//  shadow.hpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 7/30/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef shadow_hpp
#define shadow_hpp

#include "shader.hpp"

namespace engine
{
    
    #define COLOR_TEXTURE_UNIT                      GL_TEXTURE0
    #define COLOR_TEXTURE_UNIT_INDEX                0
    #define SHADOW_TEXTURE_UNIT                     GL_TEXTURE1
    #define SHADOW_TEXTURE_UNIT_INDEX               1
    #define CASCACDE_SHADOW_TEXTURE_UNIT0           SHADOW_TEXTURE_UNIT
    #define CASCACDE_SHADOW_TEXTURE_UNIT0_INDEX     SHADOW_TEXTURE_UNIT_INDEX
    #define CASCACDE_SHADOW_TEXTURE_UNIT1           GL_TEXTURE2
    #define CASCACDE_SHADOW_TEXTURE_UNIT1_INDEX     2
    #define CASCACDE_SHADOW_TEXTURE_UNIT2           GL_TEXTURE3
    #define CASCACDE_SHADOW_TEXTURE_UNIT2_INDEX     4
    
    class Shadow
    {
    public:
        
        Shadow(uint width, Shader* rsh);
        
        virtual ~Shadow();
        
        void UpdateMatrix(const glm::mat4 matrix);
        
        void UpdateModel(const glm::mat4 model);
        
    protected:
        Shader* dShader = nullptr;  // depth shader
        Shader* rShader = nullptr;  // render shader
        GLuint fbo;
        uint width;
    };
    
    
    class GeneralShadow : public Shadow
    {
    public:
        
        GeneralShadow(uint width, Shader* rsh);
        
        virtual ~GeneralShadow();
        
        bool Init();
        
        void BindForWriting();
        
        void BindForReading(GLenum TextureUnit);
        
    protected:
        GLuint map;
    };
    
    
    class CascadedShadow : public Shadow
    {
        
    public:
        
        CascadedShadow(uint width, Shader* rsh);
        
        virtual ~CascadedShadow();
        
        bool Init();
        
        void BindForWriting(uint CascadeIndex);
        
        void BindForReading();
        
        
        
    protected:
        GLuint map[3];
    };

}


#endif /* shadow_hpp */
