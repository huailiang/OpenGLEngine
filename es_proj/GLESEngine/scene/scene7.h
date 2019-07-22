//
//  scene7.h
//  GLESEngine
//
//  Created by 彭怀亮 on 7/22/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene7_h
#define scene7_h

#include "vrscene.h"
#ifdef _GLES_
#include "IARInterface.h"
#endif

class Scene7 : public VRScene
{
    
public:
    
    ~Scene7()
    {
        SAFE_DELETE(pick);
        SAFE_DELETE(match);
    }
    
    int getType() { return TY_Scene7; }
    
    void InitLight() { light = new DirectLight(vec3(1.0f), vec3(0.0f, 0.0f, -2.0f)); }
    
    bool drawShadow() { return false; }
    
    virtual void InitScene()
    {
        VRScene::InitScene();
        std::cout<<"scene7 initial"<<std::endl;
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        pick = new engine::UIButton(vec2(720, 360), vec3(1,1,0), 0.6f, "pick", 0);
        match = new  engine::UIButton(vec2(720, 330), vec3(1,1,0), 0.6f, "match", 1);
        pick->RegistCallback(OnClick, this);
        match->RegistCallback(OnClick, this);
    }
    
    
    void DrawAR(const std::vector<Transformation> &transforms)
    {
        VRScene::DrawAR(transforms);
    }
    
    
    void StartPick()
    {
        if(arPtr)
        {
            arPtr->GetAlbumPicker();
            
            return;
            
//            std::cout<<"pick finish: "<<std::endl;
//            float *width = nullptr, *height = nullptr;
//            void* data = arPtr->GetImageData(width, height);
//            std::cout<<"width:"<<*width<<" height:"<<*height<<std::endl;
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
    }
    
    void StartMatch()
    {
        // 
    }
    
    void Process(IARInterface* ar)
    {
        arPtr = ar;
    }
    
private:
    
    static void OnClick(engine::UIEvent* contex, void* arg)
    {
        Scene7* scene = (Scene7*)(arg);
        int evtid = contex->evtid;
        if(evtid == 0) scene->StartPick();
        if(evtid == 1) scene->StartMatch();
    }
    
private:
    engine::UIButton* pick, *match;
    IARInterface* arPtr = nullptr;
};


#endif /* scene7_h */
