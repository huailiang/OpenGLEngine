#ifndef GeometryTypes_hpp
#define GeometryTypes_hpp

#include <glm/glm.hpp>
#include <iostream>


struct Transformation
{
    Transformation();
    Transformation(const glm::mat3& r, const glm::vec3& t);

    glm::mat3& r();
    
    glm::vec3&  t();

    const glm::mat3& r() const;
    
    const glm::vec3&  t() const;

    glm::mat4 getMat44() const;

    Transformation getInverted() const;
    
    
private:
  glm::mat3 m_rotation;
  glm::vec3  m_translation;
    
};


#endif
