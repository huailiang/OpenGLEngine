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
    matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
}

ImageRecognition::~ImageRecognition() { }

std::vector<cv::Point2f> ImageRecognition::Keypoint2Point(std::vector<cv::KeyPoint> keypoints)
{
    std::vector<cv::Point2f> vectorOfPoints;
    for(unsigned i = 0; i < keypoints.size(); i++) {
        vectorOfPoints.push_back(keypoints[i].pt);
    }
    return vectorOfPoints;
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
    std::cout<<"compute keypoints: "<<referenceKeypoints.size()<<" descriptor:"<<referenceDescriptors.size<<std::endl;
    MatchImage("/Users/penghuailiang/Pictures/IMG_0013.jpg");
    MatchImage("/Users/penghuailiang/Documents/tex/style2.jpg");
    MatchImage("/Users/penghuailiang/Pictures/test.png");
    MatchImage("/Users/penghuailiang/Pictures/test2.JPG");
    DrawKeypionts(image);
}


void ImageRecognition::DrawKeypionts(cv::Mat image)
{
    for(int i=0;i<referenceKeypoints.size();i++)
    {
        cv::circle(image, referenceKeypoints[i].pt, 2, cv::Scalar(255,0,0));
    }
    cv::imshow("show", image);
    cv::waitKey(0);
}


void ImageRecognition::MatchImage(const string path)
{
    cv::Mat mat = cv::imread(path);
    MatchImage(mat);
}


void ImageRecognition::MatchImage(cv::Mat image)
{
    std::vector<cv::KeyPoint> queryKeypoints;
    cv::Mat descriptor;
    cornerDetector->detectAndCompute(image, cv::noArray(), queryKeypoints, descriptor);
    
    if(queryKeypoints.size()<=0) exit(2);
    
    std::vector<cv::KeyPoint> sourceMatches,queryMatches;

    std::vector<std::vector<cv::DMatch> > descriptorMatches;
    matcher->knnMatch(referenceDescriptors, descriptor, descriptorMatches, 2);
    for (unsigned i = 0; i < descriptorMatches.size(); i++)
    {
        if (descriptorMatches[i][0].distance < NN_MATCH_RATIO * descriptorMatches[i][1].distance)
        {
            sourceMatches.push_back(referenceKeypoints[descriptorMatches[i][0].queryIdx]);
            queryMatches.push_back(queryKeypoints[descriptorMatches[i][0].trainIdx]);
        }
    }
    
    if (sourceMatches.size() >= MIN_INLIER_COUNT) {
        std::cout<<" key points matched"<<sourceMatches.size()<<std::endl;
    }
    else
    {
        std::cerr<<"Very few kps match, cannot proceed further!"<<sourceMatches.size()<<std::endl;
    }

}

