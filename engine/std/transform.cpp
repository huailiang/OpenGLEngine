//
//  transform.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 8/9/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "transform.hpp"
#include "../profile.h"

Transform::Transform() :
m_pos(vec3(0)),
m_angle(vec3(0)),
m_scale(vec3(1))
{ }

Transform::Transform(vec3 pos) :
m_pos(pos),
m_angle(vec3(0)),
m_scale(vec3(1))
{ }

Transform::Transform(vec3 pos, float angle, vec3 scale) :
m_pos(pos),
m_angle(vec3(0, angle, 0)),
m_scale(scale)
{ }

Transform::Transform(vec3 pos, vec3 angle, vec3 scale) :
m_pos(pos),
m_angle(angle),
m_scale(scale)
{ }

Transform::Transform(vec3 pos, qua<float> rot, vec3 scale) :
m_pos(pos),
m_angle(glm::eulerAngles(rot)),
m_scale(vec3(scale))
{ }

Transform::~Transform() { }

void Transform::setPos(vec3 pos)
{
    m_pos = pos;
}

vec3 Transform::getPos()
{
    return m_pos;
}

vec3 Transform::Move(vec3 move)
{
    m_pos += move;
    return m_pos;
}

vec3 Transform::MoveX(float x)
{
    m_pos.x += x;
    return  m_pos;
}

vec3 Transform::MoveY(float y)
{
    m_pos.y += y;
    return  m_pos;
}

vec3 Transform::MoveZ(float z)
{
    m_pos.z += z;
    return m_pos;
}

void Transform::setRotate(float x, float y, float z, float w)
{
    qua<float> rot(x,y,z,w);
    setRotate(rot);
}

void Transform::setRotate(qua<float> rot)
{
    m_angle = glm::eulerAngles(rot);
}

void Transform::setAngle(vec3 angle)
{
    m_angle = angle;
}

qua<float> Transform::getRotate()
{
    m_rot = glm::qua<float>(glm::radians(m_angle));
    return m_rot;
}

vec3 Transform::getAngles()
{
    return m_angle;
}

void Transform::RotateX(float angle)
{
    m_angle.x += angle;
}

void Transform::RotateY(float angle)
{
    m_angle.y += angle;
}

void Transform::RotateZ(float angle)
{
    m_angle.z += angle;
}

void Transform::setScale(float scale)
{
    m_scale = vec3(scale, scale, scale);
}

vec3 Transform::getScale()
{
    return m_scale;
}

vec3 Transform::Scale(float scale)
{
    return vec3(scale) * m_scale;
}

vec3 Transform::Scale(vec3 scale)
{
    return scale * m_scale;
}

mat4 Transform::getWorldMatrix()
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_pos);
    m_rot = glm::qua<float>(glm::radians(m_angle));
    model = glm::mat4_cast(m_rot) * model;
    model = glm::scale(model, m_scale);
    return model;
}

float Transform::Distance(Transform& tf)
{
    vec3 pos1 = tf.getPos();
    return glm::distance(pos1, m_pos);
}

void Transform::LookAt(Transform& tf)
{
    LookAt(tf, vec3(0,1,0));
}

void Transform::LookAt(const vec3 pos)
{
    LookAt(pos, vec3(0,1,0));
}

void Transform::LookAt(Transform& tf, const vec3 up)
{
    LookAt(tf.getPos(), up);
}

void Transform::LookAt(const vec3 pos, const vec3 up)
{
    vec3 dir = pos - getPos();
    m_rot = glm::quatLookAt(dir, up);
    m_angle = glm::eulerAngles(m_rot);
}
