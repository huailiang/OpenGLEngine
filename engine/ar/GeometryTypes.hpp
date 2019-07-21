#ifndef GeometryTypes_hpp
#define GeometryTypes_hpp


#include <glm/glm.hpp>
#include <iostream>

struct Matrix44
{
  union
  {
    float data[16];
    float mat[4][4];
  };
  
  Matrix44 getTransposed() const;
  Matrix44 getInvertedRT() const;
  static Matrix44 identity();
};

struct Matrix33
{
  union
  {
    float data[9];
    float mat[3][3];
  };
  
  static Matrix33 identity();
  Matrix33 getTransposed() const;
};

struct Vector4
{
  float data[4];
};

struct Vector3
{
  float data[3];
  
  static Vector3 zero();
  Vector3 operator-() const;
};

struct Transformation
{
  Transformation();
  Transformation(const Matrix33& r, const Vector3& t);
  
  Matrix33& r();
  Vector3&  t();
  
  const Matrix33& r() const;
  const Vector3&  t() const;
  
    glm::mat4 getMat44() const;
  
  Transformation getInverted() const;
private:
  Matrix33 m_rotation;
  Vector3  m_translation;
};


std::ostream& operator<<(std::ostream& stream, Vector3 vec);
std::ostream& operator<<(std::ostream& stream, Vector4 vec);
std::ostream& operator<<(std::ostream& stream, Matrix33 vec);
std::ostream& operator<<(std::ostream& stream, Matrix44 vec);


#endif
