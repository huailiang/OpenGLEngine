//
//  light.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "light.hpp"

namespace engine
{
    
    Light::Light(vec3 color,vec3 direction)
    {
        this->color = color;
        this->direction = direction;
    }
    
    
    void Light::UpdateX(float dx)
    {
        if(direction.x + dx < radians(60.0f))
        {
            direction.x += dx;
        }
    }
    
    void Light::UpdateY(float dy)
    {
        if(direction.y + dy < radians(60.0f))
        {
            direction.y += dy;
        }
    }
    
    void DirectLight::Apply(const Material* mat)
    {
        if(mat && mat->shader) Apply(mat->shader);
    }
    
    void DirectLight::Apply(const Shader* shader)
    {
        shader->setVec3("light.color", color);
        shader->setVec3("light.direction", direction);
    }
    
    mat4 DirectLight::getViewMatrix() const
    {
        return lookAt(vec3(0,0,-1), direction, vec3(0,1,0));
    }
    
    mat4 DirectLight::getLigthSpaceMatrix(float l, float r, float b, float t, float near, float far)
    {
//        mat4 view =  getViewMatrix();
        vec3 pos = vec3((l+r)/2,(t+b)/2,near-1);
        mat4 view = lookAt(vec3((l+r)/2,(t+b)/2,(near+far)/2), direction, vec3(0,1,0));
        mat4 proj = glm::ortho(l, r, b, t, near, far);
        return proj * view;
    }
    
    LightType  DirectLight::getType() const
    {
        return LightDirect;
    }
    
    std::string DirectLight::getMacro() const
    {
        return "_DirectLight_";
    }
    
    PointLight::PointLight(vec3 color, vec3 direction, vec3 pos, vec3 constant)
    :Light(color, direction)
    {
        this->pos=pos;
        this->constant=constant;
    }
    
    mat4 PointLight::getViewMatrix() const
    {
        vec3 center = pos + direction;
        return lookAt(pos, center, vec3(0,1,0));
    }
    
    mat4 PointLight::getLigthSpaceMatrix(float l, float r, float b, float t, float near, float far)
    {
        mat4 view = getViewMatrix();
        mat4 proj = glm::ortho(l, r, b, t, near, far);
        return proj * view;
    }
    
    void PointLight::Apply(const Material* mat)
    {
        if(mat && mat->shader) Apply(mat->shader);
    }
    
    void PointLight::Apply(const Shader* shader)
    {
        shader->setVec3("light.color", this->color);
        shader->setVec3("light.direction", this->direction);
        shader->setVec3("light.constant", this->constant);
        shader->setVec3("light.position", this->pos);
    }
    
    LightType PointLight::getType() const
    {
        return LightPoint;
    }
    
    std::string PointLight::getMacro() const
    {
        return "_PointLight_";
    }
    
    SpotLight::SpotLight(vec3 color, vec3 direction, vec3 pos, vec3 constant,float cutOff,float outerCutOff)
    :PointLight(color, direction, pos,constant)
    {
        this->cutOff= cos(glm::radians(cutOff));
        this->outerCutOff = cos(glm::radians(outerCutOff));
    }
    
    void SpotLight::Apply(const Material* mat)
    {
        if(mat && mat->shader) Apply(mat->shader);
    }
    
    void SpotLight::Apply(const Shader* shader)
    {
        PointLight::Apply(shader);
        shader->setFloat("light.cutOff", this->cutOff);
        shader->setFloat("light.outerCutOff", this->outerCutOff);
    }
    
    LightType SpotLight::getType() const
    {
        return LightSpot;
    }
    
    std::string SpotLight::getMacro() const
    {
        return "_SpotLight_";
    }
    
}
