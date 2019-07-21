//
//  ImageRecognition.hpp
//  GLESEngine
//
//  Created by 彭怀亮 on 7/20/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef ImageRecognition_hpp
#define ImageRecognition_hpp

#include <opencv2/opencv.hpp>
#include "../engine.h"

using namespace engine;

#define MIN_KPS_IN_FRAME            300
#define MIN_INLIER_COUNT            30 

class ImageRecognition
{
public:
    ImageRecognition();
    ~ImageRecognition();
    
    void ProcessImage(const string path);
    void ProcessImage(cv::Mat iamge);
    void DrawKeypionts();
    
private:
    cv::Mat referenceDescriptors;
    std::vector<cv::KeyPoint> referenceKeypoints;
    cv::Ptr<cv::Feature2D> cornerDetector;
};

#endif /* ImageRecognition_hpp */
