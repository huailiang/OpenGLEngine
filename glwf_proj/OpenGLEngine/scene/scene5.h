//
//  scene5.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/26/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene5_h
#define scene5_h

#include "scene.h"

class Scene5 : public Scene
{
    
public:
    ~Scene5()
    {
        SAFE_DELETE(halo);
        SAFE_DELETE(shader);
    }
    
    int getType() { return TY_Scene5; }
    
    glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,16.0f); }
    
    void InitLight()
    {
        light = new DirectLight(vec3(1.0f), vec3(1.0f,-2.0f,2.0f));
    }
    
    void InitScene()
    {
        shader = new LightShader("model.vs", "model.fs",nullptr,Macro("VERT_TYPE","0x2111"));
        ApplyCamera(shader);
        halo = new Avatar("halo", vec3(-1.0f, -4.0f, -1.5f), vec3(1.0f), -60);
    }

    void DrawScene()
    {
        halo->Rotate(0.2f);
        halo->Draw(shader, light);
    }
    
private:
    LightShader* shader;
    Avatar *halo;
};


#endif /* scene4_h */
