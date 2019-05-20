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
    
    virtual ~Light() { }
    
    void UpdateX(float dx)
    {
        if(direction.x+dx<radians(60.0f))
        {
            direction.x+=dx;
        }
    }
    
    void UpdateY(float dy)
    {
        if(direction.y + dy <radians(60.0f))
        {
            direction.y+=dy;
        }
    }
    
    LightType virtual getType() = 0;
    
    void virtual Apply(Shader* shader) = 0;
};


class DirectLight: public Light
{
public:
    DirectLight(vec3 color,vec3 direction): Light(color, direction) {}
    
    virtual ~DirectLight() { }
    
    void Apply(Shader* shader)
    {
        shader->setVec3("light.color", color);
        shader->setVec3("light.direction", direction);
    }
    
    mat4 GetLigthSpaceMatrix(vec3 target,float near, float far, float up = 8, float left = 8)
    {
        vec3 dir = normalize(direction);
        int sc = 4;
        vec3 pos = target - vec3(sc*dir.x, sc*dir.y,sc*dir.z);
        mat4 view =  lookAt(pos, target, vec3(0,1,0));
        mat4 proj = glm::ortho(-left, left, -up, up, near, far);
        return proj * view;
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
    
    virtual ~PointLight() { }
    
    mat4 GetLigthSpaceMatrix(float near, float far, float up = 8, float left = 8)
    {
        vec3 center = pos + direction;
        mat4 view = lookAt(pos, center, vec3(0,1,0));
        mat4 proj = glm::ortho(-left, left, -up, up, near, far);
        return proj * view;
    }

    void Apply(Shader* shader)
    {
        shader->setVec3("light.color", this->color);
        shader->setVec3("light.direction", this->direction);
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
    
    SpotLight(vec3 color, vec3 direction, vec3 pos, vec3 constant,float cutOff,float outerCutOff)
                    :PointLight(color, direction, pos,constant)
    {
        this->cutOff=cutOff;
        this->outerCutOff = outerCutOff;
    }
    
    void Apply(Shader* shader)
    {
        PointLight::Apply(shader);
        shader->setFloat("light.cutOff", this->cutOff);
        shader->setFloat("light.outerCutOff", this->outerCutOff);
    }
    
    LightType getType()
    {
        return LightSpot;
    }
};

#endif /* light_h */
