#include "GeometryTypes.hpp"
#include <glm/glm.hpp>

Matrix44 Matrix44::getTransposed() const
{
  Matrix44 t;
  
  for (int i=0;i<4; i++)
    for (int j=0;j<4;j++)
      t.mat[i][j] = mat[j][i];
    
  return t;
}

Matrix44 Matrix44::identity()
{
  Matrix44 eye;
  
  for (int i=0;i<4; i++)
    for (int j=0;j<4;j++)
      eye.mat[i][j] = i == j ? 1 : 0;
  
  return eye;
}

Matrix44 Matrix44::getInvertedRT() const
{
  Matrix44 t = identity();
  
  for (int col=0;col<3; col++)
  {
    for (int row=0;row<3;row++)
    { 
      // Transpose rotation component (inversion)
      t.mat[row][col] = mat[col][row];
    }
    
    // Inverse translation component
    t.mat[3][col] = - mat[3][col];
  }
  return t;
}

Matrix33 Matrix33::identity()
{
  Matrix33 eye;
  
  for (int i=0;i<3; i++)
    for (int j=0;j<3;j++)
      eye.mat[i][j] = i == j ? 1 : 0;
  
  return eye;
}

Matrix33 Matrix33::getTransposed() const
{
  Matrix33 t;
  
  for (int i=0;i<3; i++)
    for (int j=0;j<3;j++)
      t.mat[i][j] = mat[j][i];
  
  return t;
}

Vector3 Vector3::zero()
{
  Vector3 v = { 0,0,0 };
  return v;
}

Vector3 Vector3::operator-() const
{
  Vector3 v = { -data[0],-data[1],-data[2] };
  return v;
}

Transformation::Transformation()
: m_rotation(Matrix33::identity())
, m_translation(Vector3::zero())
{
  
}

Transformation::Transformation(const Matrix33& r, const Vector3& t)
: m_rotation(r)
, m_translation(t)
{
  
}

Matrix33& Transformation::r()
{
  return m_rotation;
}

Vector3&  Transformation::t()
{
  return  m_translation;
}

const Matrix33& Transformation::r() const
{
  return m_rotation;
}

const Vector3&  Transformation::t() const
{
  return  m_translation;
}

glm::mat4 Transformation::getMat44() const
{
  glm::mat4 res(1);
  
  for (int col=0;col<3;col++)
  {
    for (int row=0;row<3;row++)
    {
      // Copy rotation component
      res[row][col] = m_rotation.mat[row][col];
    }
    
    // Copy translation component
    res[3][col] = m_translation.data[col];
  }
  
  return res;
}

Transformation Transformation::getInverted() const
{
  return Transformation(m_rotation.getTransposed(), -m_translation); 
}


std::ostream& operator<<(std::ostream& stream, Vector3 vec)
{
    return  stream<<" ("<<vec.data[0]<<","<<vec.data[1]<<","<<vec.data[2]<<") ";
}

std::ostream& operator<<(std::ostream& stream, Vector4 vec)
{
    return  stream<<" ("<<vec.data[0]<<","<<vec.data[1]<<","<<vec.data[2]<<","<<vec.data[3]<<") ";
}

std::ostream& operator<<(std::ostream& stream, Matrix33 vec)
{
     return  stream<<std::endl<<\
        vec.data[0]<<"\t"<<vec.data[1]<<"\t"<<vec.data[2]<<std::endl<< \
        vec.data[3]<<"\t"<<vec.data[4]<<"\t"<<vec.data[5]<<std::endl<< \
        vec.data[6]<<"\t"<<vec.data[7]<<"\t"<<vec.data[8]<<std::endl;
}

std::ostream& operator<<(std::ostream& stream, Matrix44 vec)
{
    return  stream<<std::endl<<\
    vec.data[0]<<"\t"<<vec.data[1]<<"\t"<<vec.data[2]<<"\t"<<vec.data[3]<<std::endl<< \
    vec.data[4]<<"\t"<<vec.data[5]<<"\t"<<vec.data[6]<<"\t"<<vec.data[7]<<std::endl<< \
    vec.data[8]<<"\t"<<vec.data[9]<<"\t"<<vec.data[10]<<"\t"<<vec.data[11]<<std::endl<< \
    vec.data[12]<<"\t"<<vec.data[13]<<"\t"<<vec.data[14]<<"\t"<<vec.data[15]<<std::endl;

}
