//
//  scene7.h
//  GLESEngine
//
//  Created by 彭怀亮 on 7/22/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene5_h
#define scene5_h

#include <opencv2/opencv.hpp>
#include "arscene.h"
#ifdef _GLES_
#include "IARInterface.h"
#endif

class Scene5 : public ARScene
{
    
public:
    
    ~Scene5()
    {
        SAFE_DELETE(pick);
        SAFE_DELETE(match);
        DELETE_TEXTURE(texID);
        if(arPtr) arPtr->OnDestroy();
    }
    
    int getType() { return TY_Scene5; }
    
    void InitLight() { light = new DirectLight(vec3(1.0f), vec3(0.0f, 0.0f, -2.0f)); }
    
    virtual void InitScene()
    {
        ARScene::InitScene();
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        cornerDetector = cv::ORB::create(750);
        matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        pick = new engine::UIButton(vec2(720, 360), vec3(1,1,0), 0.6f, "pick", 0);
        match = new  engine::UIButton(vec2(720, 330), vec3(1,1,0), 0.6f, "match", 1);
        pick->RegistCallback(OnClick, this);
        match->RegistCallback(OnClick, this);
    }
    
    void DrawScene()
    {
        DrawBackground();
        RenderQuad(texID);
    }
    
    void StartPick()
    {
        if(arPtr)
        {
            arPtr->GetAlbumPicker(OnPickerFinish, this);
        }
    }
    
    void OnPicker()
    {
        float x = 0, y = 0;
        void* data = arPtr->GetImageData(&x, &y);
        std::cout<<"width:"<<x<<" height:"<<y<<std::endl;
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        cv::Mat image(x, y, CV_8UC4, data);
        cornerDetector->detect(image, referenceKeypoints);
        std::cout<<"image key points size:"<<referenceKeypoints.size()<<std::endl;
    }
    
    void StartMatch()
    {
        std::cout<<"match finish"<<std::endl;
    }
    
    void Process(IARInterface* ar)
    {
        arPtr = ar;
    }
    
private:
    
    static void OnPickerFinish(void* arg)
    {
        Scene5* sc = (Scene5*)arg;
        sc->OnPicker();
    }
    
    static void OnClick(engine::UIEvent* contex, void* arg)
    {
        Scene5* scene = (Scene5*)(arg);
        int evtid = contex->evtid;
        if(evtid == 0) scene->StartPick();
        if(evtid == 1) scene->StartMatch();
    }
    
private:
    engine::UIButton* pick, *match;
    IARInterface* arPtr = nullptr;
    GLuint texID;
    
    cv::Mat referenceDescriptors;
    std::vector<cv::KeyPoint> referenceKeypoints;
    cv::Ptr<cv::Feature2D> cornerDetector;
    cv::Ptr<cv::DescriptorMatcher> matcher;
};


#endif /* scene7_h */
