//
//  animation.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef animation_h
#define animation_h

#include "../engine.h"


namespace engine
{
 
    #define ANI_NAME_LEN 30
    
    struct Key
    {
        float time;
        glm::vec4 rot;
        glm::vec3 pos;
    };
    
    // for tool
    struct Track
    {
        std::vector<Key> keys;
    };
    
    // for runtime
    struct XTrack
    {
        unsigned int num_key;
        Key* keys;
    };
    
    // for tool
    struct Animation
    {
        char nameLength;
        char name[ANI_NAME_LEN];
        float time;
        std::vector<Track> tracks;
        unsigned int frameCount;
    };
    
    // for runtime
    struct XAnimation
    {
        char nameLength;
        char name[ANI_NAME_LEN];
        float time;
        unsigned int num_track;
        XTrack* tracks;
        unsigned int frameCount;
    };
    
    // for tool
    struct Bone
    {
        char nameLength;
        char name[ANI_NAME_LEN];
        float rot[4]; // angle, x, y, z
        float pos[3];
        int parent;
        glm::mat4 matrix;
        glm::mat4 invbindmatrix;
        std::vector<int> childs;
    };
    
    // for runtime
    struct XBone
    {
        char nameLength;
        char name[ANI_NAME_LEN];
        float rot[4]; // angle, x, y, z
        float pos[3];
        int parent;
        glm::mat4 matrix;
        glm::mat4 invbindmatrix;
        unsigned int num_child;
        int* childs;
    };
    
    
    class Skeleton
    {
    
    public:
    
        void EvalSubtree(int id,XAnimation &ani,int frame, float weight=0);
        
        void SetPose(int animation_index,double time);
        
        void SetBindPose();
        
        Key& GetInterpolatedKey(XTrack& track,int frame,float weight,bool normalize=false);
        
    private:
        
        int num_bone, num_anim;
        
        XBone* bones;
        
        XAnimation* animations;
        
    };
}

#endif /* animation_h */
