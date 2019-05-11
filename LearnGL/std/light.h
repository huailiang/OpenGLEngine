//
//  light.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/10/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef light_h
#define light_h

#include <glm/glm.hpp>
#include "shader.h"

using namespace glm;

enum LightType
{
    LightPoint,
    LightDirect,
    LightSpot,
};

class Light
{
public:
    vec3 color;
    vec3 direction;

    Light(vec3 color,vec3 direction)
    {
        this->color=color;
        this->direction=direction;
    }
    
    LightType virtual getType() = 0;
    void virtual Attach(Shader* shader) = 0;
};


class DirectLight: public Light
{
public:
    DirectLight(vec3 color,vec3 direction): Light(color, direction) {}
    
    void Attach(Shader* shader)
    {
        shader->setVec3("light.color", color);
        shader->setVec3("light.direction", direction);
    }
    
    LightType  getType()
    {
        return LightDirect;
    }
};

class PointLight : public Light
{
public:
    vec3 pos;
    vec3 constant; //衰减系数
    
    PointLight(vec3 color, vec3 direction, vec3 pos, vec3 constant)
            :Light(color, direction)
    {
        this->pos=pos;
        this->constant=constant;
    }
    
    void Attach(Shader* shader)
    {
        shader->setVec3("light.color", this->color);
        shader->setVec3("light.direction", this->direction);
        std::cout<<"direction:"<<direction.x<<" "<<direction.y<<" "<<direction.z<<std::endl;
        shader->setVec3("light.constant", this->constant);
        shader->setVec3("light.position", this->pos);
    }
    
    LightType getType()
    {
        return LightPoint;
    }
};


class SpotLight : public PointLight
{
public:
    float cutOff;
    float outerCutOff;
    vec3 direction;
    
    SpotLight(vec3 color, vec3 pos, vec3 constant,float cutOff,float outerCutOff,vec3 direction)
                    :PointLight(color, direction, pos,constant)
    {
        this->cutOff=cutOff;
        this->outerCutOff = outerCutOff;
    }
    
    void Attach(Shader* shader)
    {
        PointLight::Attach(shader);
        shader->setFloat("light.cutOff", cutOff);
        shader->setFloat("light.outerCutOff", outerCutOff);
    }
    
    LightType getType()
    {
        return LightSpot;
    }
};

#endif /* light_h */
