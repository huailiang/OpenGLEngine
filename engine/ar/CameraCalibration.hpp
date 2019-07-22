//
//  CameraCalibration.hpp
//  GLESEngine
//
//  Created by 彭怀亮 on 7/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef CameraCalibration_hpp
#define CameraCalibration_hpp

#include "GeometryTypes.hpp"

class CameraCalibration
{
    
public:
    
    CameraCalibration();
    CameraCalibration(float fx, float fy, float cx, float cy);
    CameraCalibration(float fx, float fy, float cx, float cy, float distorsionCoeff[4]);
  
    const glm::mat3& getIntrinsic() const;
    
    const glm::vec4&  getDistorsion() const;
  
private:
    glm::mat3 m_intrinsic;
    glm::vec4  m_distorsion;
    
};

#endif
