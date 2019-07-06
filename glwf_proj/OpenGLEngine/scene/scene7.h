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
        shader = new LightShader("ibl/harmonics.vs", "ibl/harmonics.fs");
        dragon = new Avatar("dragon",vec3(0), vec3(0.6f));
        
        string sh_coef_file = WORKDIR + string("tools/data/CNTower/coefficients.txt");
        ifstream ifs(sh_coef_file);
        if (!ifs)
        throw runtime_error("open " + sh_coef_file + " failed");
        int i = 0;
        float r, g, b;
        while (ifs >> r >> g >> b)
        {
            coefs[i] = glm::vec3(r, g, b);
            i++;
        }
    }
    
    void DrawScene()
    {
        dragon->Draw(shader, light, camera);
        shader->use();
        shader->setVec3("coef", 16, coefs[0]);
    }
    
private:
    Shader *shader = nullptr;
    Avatar *dragon = nullptr;
    glm::vec3 coefs[16];

};

#endif /* scene7_h */
