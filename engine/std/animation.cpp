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
        resample=false;
    }
    
    Skeleton::~Skeleton()
    {
        delete [] animations;
        delete [] bones;
        num_anim = 0;
        num_bone = 0;
        current = nullptr;
    }
    
    void Skeleton::EvalSubtree(int id, XAnimation &ani,int frame, float weight)
    {
        XBone &b=bones[id];
        glm::mat4 a(1),m(1);
        
        glm::vec3 pos(b.pos[0],b.pos[1],b.pos[2]);
        m = glm::rotate(m, b.rot[0], glm::vec3(b.rot[1], b.rot[2], b.rot[3]));
//        std::cout<<id<<"\t"<< ani.tracks[id].num_key<<"\tframe:"<<frame<<std::endl;
        if(ani.tracks[id].num_key > frame) // add animated pose if track available
            if(frame>=0)
            {
                Key &k = GetInterpolatedKey(ani.tracks[id],frame,weight);
                a = glm::rotate(a, k.rot[0], glm::vec3(k.rot[1],k.rot[2],k.rot[3]));
                pos=pos + glm::vec3(k.pos[0],k.pos[1],k.pos[2]);
                m = m * a;
            }
        m = glm::translate(m, pos);
        b.matrix=b.parent>=0 ? bones[b.parent].matrix * m : m;
        loop0i(b.num_child) EvalSubtree(b.childs[i],ani,frame,weight);
    }
    
    void Skeleton::SetPose(int animation_index)
    {
        if(animation_index>=num_anim) std::cerr<<"animation index out of range "<<animation_index<<std::endl;
        if(!resample) { ResampleAnimationTracks(20); SetBindPose(); resample=true; }
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
            playtime = 1;
            double time01=playtime/double(current->time);
            time01=time01-floor(time01);
            float frame=(current->frameCount-2)*time01+1;
            
            loop0i(num_bone)  bones[i].matrix = glm::mat4(1);
            loop0i(num_bone)  if (bones[i].parent==-1) EvalSubtree((int)i,*current,int(frame),frac(frame));
            
            playtime+=deltatime;
        }
    }
    
    
    void Skeleton::Draw(Shader* shader)
    {
        this->shader = shader;
        glm::mat4 ibones[100];
        InnerPlay();
        loop(num_bone) ibones[i] = (bones+i)->matrix *  (bones+i)->invbindmatrix;
    }
    
    void Skeleton::SetBindPose()
    {
        XAnimation &ani=animations[0];
        loop0i(num_bone) bones[i].matrix = glm::mat4(1);
        loop0i(num_bone)  if (bones[i].parent==-1) EvalSubtree((int)i,ani,-1,0);
        loop0i(num_bone)  bones[i].invbindmatrix=glm::inverse(bones[i].matrix);
    }
    
    void Skeleton::ResampleAnimationTracks(double frames_per_second)
    {
        loop0i(num_anim)
        loop0j(animations[i].num_track)
        if(animations[i].tracks[j].num_key>0)
        {
            XTrack dst;
            XTrack &src = animations[i].tracks[j];
            float length = animations[i].time;
            int newframecount = length*frames_per_second;
            int src_frame = 0;
            
            dst.num_key = newframecount;
            dst.keys = new Key[newframecount];
            for (int k=0; k<newframecount; k++) {
                double time=k*length/double(newframecount-1);
                while(src_frame<src.num_key && time>src.keys[src_frame].time) src_frame++;
                
                int src_frame_1 = clamp(src_frame-1, 0, src.num_key-1);
                int src_frame_2 = clamp(src_frame, 0, src.num_key-1);
                float t1=src.keys[src_frame_1].time;
                float t2=src.keys[src_frame_2].time;
                float w = (time-t1)/(t2-t1);
                Key key=GetInterpolatedKey(src,src_frame_1,w,true);
                dst.keys[k] = key;
            }
            animations[i].tracks[j] = dst;
            animations[i].frameCount = newframecount;
        }
    }
    
    
    
    Key& Skeleton::GetInterpolatedKey(XTrack& track,int frame,float weight,bool normalize)
    {
        Key &k0=track.keys[(frame) % track.num_key];
        Key &k1=track.keys[(frame+1) % track.num_key];
        
        static Key k;
        float weight1 = 1.0 - weight;
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
