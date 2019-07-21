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
#define NN_MATCH_RATIO              0.8f
#define RANSAC_THRESH               2.5f 

class ImageRecognition
{
public:
    ImageRecognition();
    ~ImageRecognition();
    
    void ProcessImage(const string path);
    void ProcessImage(cv::Mat image);
    void DrawKeypionts(cv::Mat image);
    void MatchImage(const string path);
    void MatchImage(cv::Mat image);
    
    std::vector<cv::Point2f> Keypoint2Point(std::vector<cv::KeyPoint> keypoints);

private:
    cv::Ptr<cv::DescriptorMatcher> matcher;
    cv::Mat referenceDescriptors;
    std::vector<cv::KeyPoint> referenceKeypoints;
    cv::Ptr<cv::Feature2D> cornerDetector;
};

#endif /* ImageRecognition_hpp */
