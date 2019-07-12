//
//  camera.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "camera.h"

namespace engine
{

    extern uint SCR_WIDTH;
    extern uint SCR_HEIGHT;
    
    mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    mat4 Camera::GetProjMatrix() const
    {
        return perspective(radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    }

    mat4 Camera::GetVP() const
    {
        mat4 view = GetViewMatrix();
        mat4 proj = GetProjMatrix();
        return proj * view;
    }

    mat4 Camera::RotateAt(const vec3 pos, const vec3 target)
    {
        Position = pos;
        Front = normalize(target - pos);
        updateCameraVectors();
        return GetViewMatrix();
    }

    void Camera::ProcessKeyboard(const Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        updateUbo();
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        Front.x   += xoffset;
        Front.y += yoffset;
        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yoffset)
    {
        if (FOV >= 1.0f && FOV <= 45.0f) FOV -= yoffset;
        if (FOV <= 1.0f) FOV = 1.0f;
        if (FOV >= 45.0f) FOV = 45.0f;
    }


    void Camera::Attach(const Shader* shader)
    {
        if(shader->ID == 0 || shader->ID>10000)
            std::cout<<"camera shader error: "<<shader->ID<<" compiled:"<<shader->compiled<<std::endl;
        //config
        GLuint uniform = glGetUniformBlockIndex(shader->ID, "Block");
        //link
        glUniformBlockBinding(shader->ID, uniform, 0);
        //bind to ubo
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));
        //update
        updateUbo();
    }

    void Camera::InitUbo()
    {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4) + sizeof(vec3), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void Camera::updateUbo()
    {
        if(ubo)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), glm::value_ptr(GetProjMatrix()));
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), glm::value_ptr(GetViewMatrix()));
            glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), sizeof(vec3), glm::value_ptr(Position));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }

    void Camera::updateCameraVectors()
    {
        Right = normalize(glm::cross(Front, WorldUp));
        Up    = normalize(glm::cross(Right, Front));
        updateUbo();
    }

}
