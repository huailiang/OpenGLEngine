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

namespace engine
{

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
        
        mat4 GetViewMatrix();
        
        mat4 GetProjMatrix();
        
        mat4 GetVP();
        
        mat4 RotateAt(const vec3 pos, const vec3 target);
        
        void ProcessKeyboard(const Camera_Movement direction, float deltaTime);
        
        void ProcessMouseMovement(float xoffset, float yoffset);
        
        void ProcessMouseScroll(float yoffset);
        
        void Attach(Shader* shader);
        
    private:
        void InitUbo();
        
        void updateUbo();
        
        void updateCameraVectors();
        
    private:
        GLuint ubo = 0;
        
    };
    
}
#endif
