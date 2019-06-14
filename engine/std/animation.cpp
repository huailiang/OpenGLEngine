//
//  animation.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "animation.h"

namespace engine
{
    
    Skeleton::Skeleton()
    {
        playtime=0;
    }
    
    Skeleton::~Skeleton()
    {
        delete [] animations;
        delete [] bones;
        num_anim = 0;
        num_bone = 0;
    }
    
    void Skeleton::EvalSubtree(int id,XAnimation &ani,int frame, float weight)
    {
        XBone &b=bones[id];
        glm::mat4 a(1),m(1);
        
        // bind pose : default
        glm::vec3 pos(b.pos[0],b.pos[1],b.pos[2]);
        m = glm::rotate(m, b.rot[0], glm::vec3(b.rot[1], b.rot[2], b.rot[3]));

        if(ani.tracks[id].num_key>frame) // add animated pose if track available
        if(frame>=0)
        {
            Key &k=GetInterpolatedKey(ani.tracks[id],frame,weight);
            a = glm::rotate(a, k.rot[0], glm::vec3(k.rot[1],k.rot[2],k.rot[3]));
            pos=pos+glm::vec3(k.pos[0],k.pos[1],k.pos[2]);
            m=a*m;
        }
        m = glm::translate(m, pos);
        b.matrix=b.parent>=0 ? m*bones[b.parent].matrix : m;
        
        loop0i(b.num_child) EvalSubtree(b.childs[i],ani,frame,weight);
    }
    
    void Skeleton::SetPose(int animation_index)
    {
        if(animation_index>=num_anim) std::cerr<<"animation index out of range "<<animation_index<<std::endl;
        
        current = &animations[animation_index];
        playtime = 0;
    }
    
    void Skeleton::PlayAnim(std::string anim)
    {
        loop(num_anim) if((animations+i)->name == anim) { SetPose(i); break; }
    }
    
    void Skeleton::InnerPlay()
    {
        if(current)
        {
            double time01=playtime/double(current->time);
            time01=time01-floor(time01);
            float frame=(current->frameCount-2)*time01+1;
            
            loop0i(num_bone)  bones[i].matrix = glm::mat4(1);
            loop0i(num_bone)  if (bones[i].parent==-1) EvalSubtree((int)i,*current,int(frame),frac(frame));
        }
    }
    
    
    void Skeleton::Draw(Shader* shader)
    {
        this->shader = shader;
        shader->setMat4("bones", bones->matrix);
        InnerPlay();
    }
    
    void Skeleton::SetBindPose()
    {
        XAnimation &ani=animations[0];
        loop0i(num_bone) bones[i].matrix = glm::mat4(1);
        loop0i(num_bone)  if (bones[i].parent==-1) EvalSubtree((int)i,ani,-1,0);
        loop0i(num_bone)  bones[i].invbindmatrix=bones[i].matrix;
        loop0i(num_bone)  glm::inverse(bones[i].invbindmatrix);
    }

    
    Key& Skeleton::GetInterpolatedKey(XTrack& track,int frame,float weight,bool normalize)
    {
        Key &k0=track.keys[(frame) % track.num_key];
        Key &k1=track.keys[(frame+1) % track.num_key];
        
        static Key k;
        float weight1=1.0-weight;
        loop(3) k.pos[i]=k0.pos[i]*weight1+k1.pos[i]*weight;
        loop(4) k.rot[i]=k0.rot[i]*weight1+k1.rot[i]*weight;
        
        if(normalize)
        {
            glm::vec3 axis(k.rot[1],k.rot[2],k.rot[3]);
            glm::normalize(axis);
            k.rot[1]=axis.x;
            k.rot[2]=axis.y;
            k.rot[3]=axis.z;
        }
        return k;
    }
    
    
    void Skeleton::PrintInfo()
    {
        std::cout<<"animation count: "<<num_anim<<" bone count:"<<num_bone<<std::endl;
        loop(num_anim) std::cout<<i<<" "<<(animations+i)->name<<" frames:"<<(animations+i)->frameCount<<std::endl;
        loop(num_bone) std::cout<<i<<" "<<(bones+i)->name<<" childs:"<<(bones+i)->num_child<<std::endl;
    }
    
    
    void Skeleton::getAnimations(std::string* names)
    {
        names = new std::string[num_anim];
        loop(num_anim) names[i] = (animations+i)->name;
    }
    
}
