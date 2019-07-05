//
//  harmonics.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 7/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef harmonics_h
#define harmonics_h

#include <vector>
#include <array>
#include <opencv2/core.hpp>
#include "util.h"

namespace tool
{
    
    class Harmonics
    {
        
    public:
        
        Harmonics(int degree);
        
        void Evaluate(const std::vector<Vertex>& vertices);
        
        Vec3 Render(const Vec3& pos);
        
        std::array<cv::Mat, 6> RenderCubemap(int width, int height);
        
        
    public:
        
        std::vector<Vec3> getCoefficients()const { return coefs; }
        
    private:
        
        std::vector<float> Basis(const Vec3& pos);
        
    private:
        int degree_;
        std::vector<Vec3> coefs;
        std::vector<float> factorial;
    };

    
}


#endif /* harmonics_hpp */
