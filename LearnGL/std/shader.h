//
//  shader.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/12/14.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H

#include "common.h"

class Shader
{
public:
    unsigned int ID;
    
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, std::string macro= "");
    
    void use();
    
    void setBool(const std::string &name, bool value) const;
    
    void setInt(const std::string &name, int value) const;
    
    void setFloat(const std::string &name, float value) const;
    
    void setVec2(const std::string &name, const glm::vec2 &value) const;

    void setVec2(const std::string &name, float x, float y) const;
    
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setVec3(const std::string &name, float x, float y, float z) const;
    
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    
    void setVec4(const std::string &name, float x, float y, float z, float w);
    
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    
private:
    
    std::string pre_process(const std::string& source,const std::string macro);
    
    std::string preprocess(const std::string& source,const std::string macro, int level);
    
    std::string openFile(const char* name);
    
    void save(std::string text, const char* name);
    
    void checkCompileErrors(GLuint shader, std::string type);
    
private:
    std::vector<std::string> headers;
};


class LightShader : public Shader
{
public:    
    LightShader(const char* vertexPath,
                const char* fragmentPath,
                const char* geometryPath = nullptr,
                std::string macro= "",
                glm::vec3 ambinent = glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f),
                float shiness=32.0f)
        :Shader(vertexPath,fragmentPath,geometryPath, macro)
    {
        this->ambinent = ambinent;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shiness;
        
        ApplyLight();
    }
    
    void ApplyLight()
    {
        use();
        setVec3("material.ambient", ambinent);
        setVec3("material.diffuse", diffuse);
        setVec3("material.specular", specular);
        setFloat("material.shininess", shininess);
    }
    
private:
    glm::vec3 ambinent, diffuse, specular;
    float shininess;
};
#endif
