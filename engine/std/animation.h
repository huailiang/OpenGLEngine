//
//  animation.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef animation_h
#define animation_h

#include "shader.h"


namespace engine
{
 
    #define ANI_NAME_LEN 30
    
    struct Key
    {
        float time;
        glm::vec4 rot;
        glm::vec3 pos;
    };
    
    struct Track
    {
        std::vector<Key> keys;
    };
    
    struct XTrack
    {
        uint num_key;
        Key* keys;
    };
    
    struct Animation
    {
        std::string name;
        float time;
        std::vector<Track> tracks;
        uint frameCount;
    };
    
    struct XAnimation
    {
        std::string name;
        float time;
        uint num_track;
        XTrack* tracks;
        uint frameCount;
        
    };
    
    struct Bone
    {
        std::string name;
        float rot[4]; // angle, x, y, z
        float pos[3];
        int parent;
        glm::mat4 matrix;
        glm::mat4 invbindmatrix;
        std::vector<int> childs;
    };
    
    struct XBone
    {
        std::string name;
        float rot[4]; // angle, x, y, z
        float pos[3];
        int parent;
        glm::mat4 matrix;
        glm::mat4 invbindmatrix;
        uint num_child;
        int* childs;
    };
    
    
    class Skeleton
    {
    
    public:
        
        Skeleton();
        
        ~Skeleton();
        
        void EvalSubtree(int id,XAnimation &ani,int frame, float weight=0);
        
        void SetPose(int animation_index);
        
        void PlayAnim(std::string anim);
        
        void Pause();
        
        void Resume();
        
        void SetBindPose();
        
        Key& GetInterpolatedKey(XTrack& track,int frame,float weight,bool normalize=false);
        
        void PrintInfo();
        
        void getAnimations(std::string* names);
        
        void Draw(Shader* shader);
        
        
    private:
        
        void InnerPlay();
        
        void ResampleAnimationTracks(double frames_per_second);
        
    public:
        int num_bone, num_anim;
        XBone* bones;
        XAnimation* animations;
        
    private:
        XAnimation* current,*backup;
        Shader* shader;
        float playtime;
        bool resample;

    };
    
}

#endif /* animation_h */
