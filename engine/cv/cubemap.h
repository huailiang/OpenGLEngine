//
//  cubemap.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 7/4/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef cubemap_h
#define cubemap_h

#include <array>
#include <vector>
#include <opencv2/core.hpp>
#include "util.h"

namespace tool
{

    class Cubemap
    {
    public:
        Cubemap(std::array<std::string, 6> image_filenames);
        Cubemap(std::array<cv::Mat, 6> images);
        std::vector<Vertex> getVertices();
        cv::Mat GenExpandImage(int maxsize = 480);
        int Width()const { return images_[0].cols; }
        int Height()const { return images_[0].rows; }
        std::vector<Vertex> RandomSample(int sqrt_n);
        Vec3 Sample(const Vec3& pos);
        Vec3 Sample(float theta, float phi);
    private:
        std::array<cv::Mat, 6> images_;
    };
    
}

#endif /* cubemap_hpp */
