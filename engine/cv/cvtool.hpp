//
//  cvtool.hpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 7/04/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef cvtool_h
#define cvtool_h

#include <opencv2/opencv.hpp>
#include <string>
#include "util.h"
#include "cubemap.h"


using namespace cv;
using namespace std;


namespace tool
{
    
    void CvGray();
    
    void Recognition();
    
    string CoefficientsString(const vector<Vec3>& coefs);
    
    int CvGenerate(int argc, const char* argv[]);
    
}

#endif /* cvtool_h */
