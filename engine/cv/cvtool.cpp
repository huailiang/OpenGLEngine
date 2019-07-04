//
//  common.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 7/04/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "cvtool.h"


namespace tool
{

    void cvGray()
    {
        string path = "/Users/penghuailiang/Pictures/IMG_0012.JPG";
        Mat image = imread(path);
        namedWindow("origin");
        imshow("origin", image);
        
        Mat gray;
        cvtColor(image, gray, COLOR_RGBA2GRAY);
        namedWindow("gray");
        imshow("gray", gray);
        waitKey(0);
    }
    
}
