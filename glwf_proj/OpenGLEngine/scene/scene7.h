//
//  scene7.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 7/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene7_h
#define scene7_h

#include "scene.h"
#include "avatar.h"


class Scene7 : public Scene
{

public:

    ~Scene7()
    {
        SAFE_DELETE(shader);
        SAFE_DELETE(dragon);
    }

    int getType() { return TY_Scene7; }

    virtual bool drawShadow() { return false; }

    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void InitScene()
    {
//        shader = new LightShader("model.vs", "model.fs");
        shader = new LightShader("ibl/harmonics.vs", "ibl/harmonics.fs");
        dragon = new Avatar("dragon",vec3(0), vec3(0.6f));
    }
    
    void DrawScene()
    {
        dragon->Draw(shader, light, camera);
    }
    
private:
    Shader *shader = nullptr;
    Avatar *dragon = nullptr;

};

#endif /* scene7_h */
