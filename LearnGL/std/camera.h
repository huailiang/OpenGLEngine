//
//  camera.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "../common.h"
#include "shader.h"

using namespace glm;

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float SPEED       =  1.0f;
const float SENSITIVITY =  0.002f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    float MovementSpeed;
    float MouseSensitivity;
    float FOV;

    // Constructor with vectors
    Camera(const vec3 position = vec3(0.0f, 0.0f, 0.0f), const vec3 up = vec3(0.0f, 1.0f, 0.0f)) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), FOV(ZOOM)
    {
        Position = position;
        WorldUp = up;
        InitUbo();
        updateCameraVectors();
    }
    
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), FOV(ZOOM)
    {
        Position = vec3(posX, posY, posZ);
        WorldUp = vec3(upX, upY, upZ);
        InitUbo();
        updateCameraVectors();
    }
    
    ~Camera()
    {
        glDeleteBuffers(1, &ubo);
    }
    
    mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    
    mat4 GetProjMatrix()
    {
        return perspective(radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    }
    
    mat4 GetVP()
    {
        mat4 view = GetViewMatrix();
        mat4 proj = GetProjMatrix();
        return proj * view;
    }
    
    mat4 RotateAt(const vec3 pos, const vec3 target)
    {
        Position = pos;
        Front = normalize(target - pos);
        updateCameraVectors();
        return GetViewMatrix();
    }

    void ProcessKeyboard(const Camera_Movement direction, float deltaTime)
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

    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        Front.x   += xoffset;
        Front.y += yoffset;
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        if (FOV >= 1.0f && FOV <= 45.0f) FOV -= yoffset;
        if (FOV <= 1.0f) FOV = 1.0f;
        if (FOV >= 45.0f) FOV = 45.0f;
    }
    
    
    void Attach(Shader* shader)
    {
        //config
        unsigned int uniform = glGetUniformBlockIndex(shader->ID, "Block");
        //link
        glUniformBlockBinding(shader->ID, uniform, 0);
        //bind to ubo
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));
        //update
        updateUbo();
    }
    
    
private:
    void InitUbo()
    {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4) + sizeof(vec3), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    void updateUbo()
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

    void updateCameraVectors()
    {
        Right = normalize(glm::cross(Front, WorldUp));
        Up    = normalize(glm::cross(Right, Front));
        updateUbo();
    }
    
private:
    unsigned int ubo = 0;
    
};
#endif
