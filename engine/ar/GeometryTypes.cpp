#include "GeometryTypes.hpp"


Transformation::Transformation() : m_rotation(glm::mat3(1)), m_translation(glm::vec3(0)) { }

Transformation::Transformation(const glm::mat3& r, const glm::vec3& t) : m_rotation(r), m_translation(t) { }

glm::mat3& Transformation::r()
{
    return m_rotation;
}

glm::vec3&  Transformation::t()
{
    return  m_translation;
}

const glm::mat3& Transformation::r() const
{
    return m_rotation;
}

const glm::vec3&  Transformation::t() const
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
            res[row][col] = m_rotation[row][col];
        }
        res[3][col] = m_translation[col];
    }
    return res;
}

Transformation Transformation::getInverted() const
{
    return Transformation( glm::transpose(m_rotation), -m_translation);
}
