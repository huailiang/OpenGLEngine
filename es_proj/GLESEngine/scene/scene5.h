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
#define MIN_INLIER_COUNT            16
#define NN_MATCH_RATIO              0.8f
#define RANSAC_THRESH               2.5f

class Scene5 : public ARScene
{
    
public:
    
    ~Scene5()
    {
        SAFE_DELETE(pick);
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
        state = new engine::UILabel(vec2(640, 440), vec3(1,0,0), 0.7f, "");
        pick = new engine::UIButton(vec2(640, 360), vec3(1,1,0), 1.0f, "pick1",1);
        pick->RegistCallback(OnClick, this);
    }
    
    void DrawScene()
    {
        DrawBackground();
        RenderQuad(texID);
        
        if(cvmatch && !captrue)
        {
            cv::Mat cam_img((int)camFrame.width, (int)camFrame.height, CV_8UC4, camFrame.data);
            std::vector<cv::KeyPoint> queryKeypoints;
            cv::Mat descriptor;
            cornerDetector->detectAndCompute(cam_img, cv::noArray(), queryKeypoints, descriptor);
            if(queryKeypoints.size() <= 0) return;
            int matchcnt = 0;
            std::vector<std::vector<cv::DMatch>> descriptorMatches;
            matcher->knnMatch(referenceDescriptors, descriptor, descriptorMatches, 2);
            for (unsigned i = 0; i < descriptorMatches.size(); i++)
            {
                if (descriptorMatches[i][0].distance < NN_MATCH_RATIO * descriptorMatches[i][1].distance) matchcnt++;
            }
            size_t s2 = referenceKeypoints.size();
            size_t s3 = queryKeypoints.size();
            state->setText("key points:" + std::to_string(matchcnt)+":"+std::to_string(s2)+":"+std::to_string(s3));
            if (matchcnt >= MIN_INLIER_COUNT) {
                state->setText(" key points matched "+std::to_string(matchcnt));
                captrue = true;
            }
        }
    }
    
    void StartPick(int eid)
    {
        if(arPtr) arPtr->GetAlbumPicker(OnPickerFinish, this, eid);
    }
    
    void OnPicker(int eid)
    {
        float x = 0, y = 0;
        void* data = arPtr->GetImageData(&x, &y);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        cv::Mat image(x, y, CV_8UC4, data);
        cornerDetector->detectAndCompute(image, cv::noArray(), referenceKeypoints, referenceDescriptors);
        std::cout<<eid<<" width:"<<x<<" height:"<<y<<"compute keypoints: "<<referenceKeypoints.size()<<" descriptor:"<<referenceDescriptors.size<<std::endl;
        cvmatch = true;
        captrue  = false;
    }
    
    
    void RenderAR()
    {
        std::cout<<"matched:"<< transforms.size()<<std::endl;
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
    
    static void OnPickerFinish(int eid, void* arg)
    {
        Scene5* sc = (Scene5*)arg;
        sc->OnPicker(eid);
    }
    
    static void OnClick(engine::UIEvent* contex, void* arg)
    {
        Scene5* scene = (Scene5*)(arg);
        auto eid = contex->evtid;
        scene->StartPick(eid);
    }
    
private:
    engine::UIButton *pick;
    engine::UILabel *state;
    GLuint texID;
    Shader* arShader;
    GLuint arVao, arVBo;
    
    cv::Mat referenceDescriptors;
    std::vector<cv::KeyPoint> referenceKeypoints;
    cv::Ptr<cv::Feature2D> cornerDetector;
    cv::Ptr<cv::DescriptorMatcher> matcher;
    bool cvmatch;
};


#endif /* scene7_h */
