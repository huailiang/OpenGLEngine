//
//  light.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/10/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef light_h
#define light_h

#include <glm/glm.hpp>
#include "material.h"

using namespace glm;

namespace engine
{

    enum LightType
    {
        LightPoint,
        LightDirect,
        LightSpot,
    };

    class Light
    {

    public:

        Light(vec3 color,vec3 direction);
        
        virtual ~Light() { }
        
        void UpdateX(float dx);
        
        void UpdateY(float dy);
        
        LightType virtual getType() const = 0;
        
        std::string virtual getMacro() const = 0;
        
        void virtual Apply(const Shader* shader) = 0;
        
        void virtual Apply(const Material* mat) = 0;
        
    public:
        vec3 color;
        vec3 direction;
    };


    class DirectLight: public Light
    {
        
    public:
        
        DirectLight(vec3 color,vec3 direction): Light(color, direction) {}
        
        virtual ~DirectLight() { }
        
        void Apply(const Shader* shader);
        
        void Apply(const Material* mat);
        
        mat4 GetLigthSpaceMatrix(vec3 target,float near, float far, float up = 8, float left = 8);

        LightType  getType() const;
        
        std::string getMacro() const;
    };

    class PointLight : public Light
    {
        
    public:
        
        PointLight(vec3 color, vec3 direction, vec3 pos, vec3 constant);
        
        virtual ~PointLight() { }
        
        mat4 GetLigthSpaceMatrix(float near, float far, float up = 8, float left = 8);
        
        void Apply(const Shader* shader);
        
        void Apply(const Material* mat);
        
        LightType getType() const;
        
        std::string getMacro() const;
        
    public:
        vec3 pos;
        vec3 constant; //衰减系数
    };


    class SpotLight : public PointLight
    {
        
    public:
        
        SpotLight(vec3 color, vec3 direction, vec3 pos, vec3 constant,float cutOff,float outerCutOff);
        
        void Apply(const Shader* shader);
        
        void Apply(const Material* mat);
        
        LightType getType() const;
        
        std::string getMacro() const;
        
    public:
        float cutOff;
        float outerCutOff;
        vec3 direction;
    };
    
}

#endif /* light_h */
