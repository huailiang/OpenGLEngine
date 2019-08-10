//
//  transform.hpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 8/9/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef transform_hpp
#define transform_hpp

#include "../engine.h"
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class Transform
{
    
public:
    
    Transform();
    
    Transform(vec3 pos);
    
    Transform(vec3 pos, float angle, vec3 scale);
    
    Transform(vec3 pos, vec3 angle, vec3 scale);
    
    Transform(vec3 pos, qua<float> rot, vec3 scale);
    
    virtual ~Transform();
    
    void setPos(vec3 pos);
    
    vec3 getPos();
    
    vec3 Move(vec3 move);
    
    vec3 MoveX(float x);
    
    vec3 MoveY(float y);
    
    vec3 MoveZ(float z);
    
    void setRotate(float x, float y, float z, float w);
    
    void setRotate(qua<float> rot);
    
    void setAngle(vec3 angle);
    
    qua<float> getRotate();
    
    vec3 getAngles();
    
    void RotateX(float angle);
    
    void RotateY(float angle);
    
    void RotateZ(float angle);
    
    void setScale(float scale);
    
    vec3 getScale();
    
    vec3 Scale(float scale);
    
    vec3 Scale(vec3 scale);
    
    mat4 getWorldMatrix();
    
    float Distance(Transform& tf);
    
    void LookAt(const vec3 pos);
    
    void LookAt(const vec3 pos, const vec3 up);
    
    void LookAt(Transform& tf);
    
    void LookAt(Transform& tf, const vec3 up);
    
private:
    vec3 m_pos;
    vec3 m_scale;
    vec3 m_angle;
    qua<float> m_rot;
    mat4 m_world;
};

#endif /* transform_hpp */
