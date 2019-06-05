//
//  avatar.hpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef avatar_h
#define avatar_h

#include "camera.h"
#include "light.h"
#include "common.h"
#include "material.h"
#include "shader.h"
#include "tool.h"

using namespace std;

class Avatar
{
    
public:
    Avatar(const char* name);
    
    ~Avatar();
    
    mat4 GetModelMatrix(vec3 pos, vec3 scale, float angle);
    
    void Draw(Shader* shader, Camera* camera, Light* light, vec3 pos, vec3 scale, float angl);
    
    void DrawShadow(Shader* shader, Camera* camera, Light* light, vec3 pos, vec3 scale, float angle);
    
private:
    vector<Material*> materials;
};

#endif /* avatar_h */
