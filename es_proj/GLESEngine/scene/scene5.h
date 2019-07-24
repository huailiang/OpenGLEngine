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

#define MIN_KPS_IN_FRAME            300
#define MIN_INLIER_COUNT            30
#define NN_MATCH_RATIO              1.8f
#define RANSAC_THRESH               2.5f

class Scene5 : public ARScene
{
    
public:
    
    ~Scene5()
    {
        SAFE_DELETE(pick);
        SAFE_DELETE(match);
        DELETE_TEXTURE(texID);
        SAFE_DELETE(arShader);
        glDeleteVertexArrays(1, &arVao);
        glDeleteBuffers(1, &arVBo);
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
        
        arShader = new Shader("debug.vs","debug.fs");
        arShader->attach("_AR_");
        InitCube(&arVao, &arVBo, arShader);
        arShader->compile();

        cornerDetector = cv::ORB::create(750);
        matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
        cvmatch =false;
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        state = new engine::UILabel(vec2(680, 440), vec3(1,0,0), 0.7f, "state info");
        pick = new engine::UIButton(vec2(720, 360), vec3(1,1,0), 1.f, "pick", 0);
        match = new  engine::UIButton(vec2(720, 310), vec3(1,1,0), 1.f, "match", 1);
        pick->RegistCallback(OnClick, this);
        match->RegistCallback(OnClick, this);
    }
    
    void DrawScene()
    {
        if(matched) return;
        DrawBackground();
        RenderQuad(texID);
        
        if(cvmatch)
        {
            cv::Mat qimg((int)camFrame.width, (int)camFrame.height, CV_8UC4, camFrame.data);
            std::vector<cv::KeyPoint> queryKeypoints;
            cv::Mat descriptor;
            cornerDetector->detectAndCompute(qimg, cv::noArray(), queryKeypoints, descriptor);
            
            if(queryKeypoints.size() <= 0) return;
            
            std::vector<cv::KeyPoint> sourceMatches;
            std::vector<std::vector<cv::DMatch>> descriptorMatches;
            matcher->knnMatch(referenceDescriptors, descriptor, descriptorMatches, 2);
            for (unsigned i = 0; i < descriptorMatches.size(); i++)
            {
                if (descriptorMatches[i][0].distance < NN_MATCH_RATIO * descriptorMatches[i][1].distance)
                {
                    sourceMatches.push_back(referenceKeypoints[descriptorMatches[i][0].queryIdx]);
                }
            }
            size_t s1 = sourceMatches.size();
            size_t s2 = referenceKeypoints.size();
            size_t s3 = queryKeypoints.size();
            state->setText("key points:" + std::to_string(s1)+":"+std::to_string(s2)+":"+std::to_string(s3));
            if (sourceMatches.size() >= MIN_INLIER_COUNT) {
                std::cout<<" key points matched "<<std::endl;
                matched = true;
            }
        }
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
    }
    
    void StartMatch()
    {
        std::cout<<"match finish"<<std::endl;
        cvmatch = true;
        matched  = false;
    }
    
    
    void RenderAR()
    {
        loop(transforms.size())
        {
            glm::mat4 view = transforms[i].getMat44(); //camera's position & rotation
            view = reverse * view;
            arShader->use();
            arShader->setMat4("view",  view);
            arShader->setMat4("proj", proj);
            glBindVertexArray(arVao);
            glDrawArrays(DRAW_MODE, 0, 36);
            glBindVertexArray(0);
        }
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
    engine::UILabel *state;
    GLuint texID;
    Shader* arShader;
    GLuint arVao, arVBo;
    
    cv::Mat referenceDescriptors;
    std::vector<cv::KeyPoint> referenceKeypoints;
    cv::Ptr<cv::Feature2D> cornerDetector;
    cv::Ptr<cv::DescriptorMatcher> matcher;
    bool cvmatch, matched;
};


#endif /* scene7_h */
