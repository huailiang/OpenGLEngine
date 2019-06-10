//
//  light.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "light.h"

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

    void DirectLight::Apply(const Shader* shader)
    {
        shader->setVec3("light.color", color);
        shader->setVec3("light.direction", direction);
    }

    mat4 DirectLight::GetLigthSpaceMatrix(vec3 target,float near, float far, float up, float left)
    {
        vec3 dir = normalize(direction);
        int sc = 4;
        vec3 pos = target - vec3(sc*dir.x, sc*dir.y,sc*dir.z);
        mat4 view =  lookAt(pos, target, vec3(0,1,0));
        mat4 proj = glm::ortho(-left, left, -up, up, near, far);
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

    mat4 PointLight::GetLigthSpaceMatrix(float near, float far, float up, float left)
    {
        vec3 center = pos + direction;
        mat4 view = lookAt(pos, center, vec3(0,1,0));
        mat4 proj = glm::ortho(-left, left, -up, up, near, far);
        return proj * view;
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
