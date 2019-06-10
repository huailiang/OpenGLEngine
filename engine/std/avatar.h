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
#include "material.h"
#include "shader.h"
#include "util.h"

using namespace std;

namespace engine
{

    class Avatar
    {
        
    public:
        Avatar(const char* name);
        
        ~Avatar();
        
        mat4 GetModelMatrix(const vec3 pos, const vec3 scale, float angle) const;
        
        void Draw(Shader* shader, Light* light, vec3 pos, vec3 scale, float angle);
        
        void DrawShadow(const Shader* shader, Light* light, vec3 pos, vec3 scale, float angle);
        
    private:

        vector<Material*> materials;

    };
    
}

#endif /* avatar_h */
