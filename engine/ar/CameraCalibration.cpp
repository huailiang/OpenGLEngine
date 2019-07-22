#include "CameraCalibration.hpp"


CameraCalibration::CameraCalibration() { }

CameraCalibration::CameraCalibration(float fx, float fy, float cx, float cy)
{
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            m_intrinsic[i][j] = 0;

    m_intrinsic[0][0] = fx;
    m_intrinsic[1][1] = fy;
    m_intrinsic[0][2] = cx;
    m_intrinsic[1][2] = cy;

    for (int i=0; i<4; i++)
        m_distorsion[i] = 0;
}

CameraCalibration::CameraCalibration(float fx, float fy, float cx, float cy, float distorsionCoeff[4])
{
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
          m_intrinsic[i][j] = 0;

    m_intrinsic[0][0] = fx;
    m_intrinsic[1][1] = fy;
    m_intrinsic[0][2] = cx;
    m_intrinsic[1][2] = cy;

    for (int i=0; i<4; i++)
        m_distorsion[i] = distorsionCoeff[i];
}

const glm::mat3& CameraCalibration::getIntrinsic() const
{
    return m_intrinsic;
}

const glm::vec4&  CameraCalibration::getDistorsion() const
{
    return m_distorsion;
}
