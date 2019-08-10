//
//  scene1.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene1_h
#define scene1_h

#include "scene.h"
#include "terrain.hpp"
#include "avatar.hpp"
#include "util.hpp"

class Scene1 : public Scene
{
    
public:
    
   ~Scene1()
    {
        SAFE_DELETE(nano);
        SAFE_DELETE(terrain);
        SAFE_DELETE(shader);
    }
    
    int getType() { return TY_Scene1; }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(-3.0f));
    }
    
    void InitScene()
    {
        shader = new LightShader("model.vs", "model.fs");
        terrain = new Terrain();
        nano = new Avatar("nanosuit",vec3(0.4f, -0.5f, -1.f), vec3(0.12f));
        nano->BindVert(shader);
        shader->compile();
        ApplyCamera(shader);
    }
    
    void DrawScene()
    {
        terrain->Draw(camera, lightMatrix, light, depthMap);
        nano->RotateY(0.2f);
        nano->Draw(shader, light, camera);
    }
    
private:
    Terrain* terrain = nullptr;
    LightShader* shader= nullptr;
    Avatar *nano = nullptr;
    
};


#endif /* scene1_h */
