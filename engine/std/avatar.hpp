//
//  avatar.hpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef avatar_h
#define avatar_h


#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "util.hpp"
#include "animation.hpp"
#include "transform.hpp"

using namespace std;

namespace engine
{

    class Avatar : public Transform
    {
        
    public:
        
        Avatar(const char* name, glm::vec3 pos = glm::vec3(0), glm::vec3 scale = glm::vec3(1), float angle = 0, Shader* shader = nullptr);
        
        ~Avatar();
        
        void BindVert(Shader* shader);
        
        void Compile(Shader* shader = nullptr);
        
        void Draw(Shader* shader, Light* light, Camera* camera);
        
        void DrawShadow(Shader* shader, Light* light, Camera* camera);
        
        void PlayAnim(std::string anim);
        
        void PauseAnim();
        
        void ResumeAnim();
        
        void SetTPose();
        
        void ChangeLOD(short ilod);
        
    private:
        
        void LoadSkeleton();
        
    private:
        
        Skeleton* skeleton = nullptr;
        
        vector<Material*> materials;
        
        vector<MeshData*> meshs;
        
        MODEL_TYPE type;
        
        const char* name;
        
    };
    
}

#endif /* avatar_h */
