//
//  ImageRecognition.cpp
//  GLESEngine
//
//  Created by 彭怀亮 on 7/20/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "ImageRecognition.hpp"


ImageRecognition::ImageRecognition()
{
    cornerDetector = cv::ORB::create(750);
}

ImageRecognition::~ImageRecognition()
{
}

void ImageRecognition::ProcessImage(const string path)
{
    cv::Mat mat = cv::imread(path);
    ProcessImage(mat);
}


void ImageRecognition::ProcessImage(cv::Mat image)
{
    cornerDetector->detect(image, referenceKeypoints);
    std::cout<<"image key points size:"<<referenceKeypoints.size()<<std::endl;
    loop(referenceKeypoints.size()) cv::circle(image, referenceKeypoints[i].pt, 5, cv::Scalar(255,0,0));
    
    cornerDetector->detectAndCompute(image, cv::noArray(), referenceKeypoints, referenceDescriptors);
    std::cout<<"compute keypoints: "<<referenceKeypoints.size()<<std::endl;
}


void ImageRecognition::DrawKeypionts()
{
    
}
