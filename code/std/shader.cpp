//
//  shader.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "shader.h"
#ifdef _GLES_
#include "FilePath.h"
#endif

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, std::string macro)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode = openFile(vertexPath);
    std::string fragmentCode = openFile(fragmentPath);
    vertexCode = pre_process(vertexCode, macro);
    fragmentCode = pre_process(fragmentCode, macro);
    std::string geometryCode;
    if(geometryPath != nullptr)  geometryCode = openFile(geometryPath);
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int geometry = 0;
    if(geometryPath != nullptr)
    {
#ifndef _GLES_
        const char * gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
#endif
    }
    
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if(geometryPath != nullptr)
        glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(geometryPath != nullptr)
        glDeleteShader(geometry);
    
#ifdef DEBUG
    save(vertexCode, vertexPath);
    save(fragmentCode, fragmentPath);
    if(geometryPath!=nullptr) save(geometryCode, geometryPath);
#endif
}


Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

std::string Shader::pre_process(const std::string& source,const std::string macro)
{
    auto str = preprocess(source,macro,0);
    headers.clear();
    return str;
}

std::string Shader::preprocess(const std::string& source,const std::string macro, int level)
{
    if(level > 32)
        throw "header inclusion depth limit reached, might be caused by cyclic header inclusion";
    std::regex re("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
    std::stringstream input;
    std::stringstream output;
    input << source;
    size_t line_number = 1;
    std::smatch matches;
    bool find = false;
    std::string line;
    
    while(std::getline(input,line))
    {
        bool title  = false;
        if(!find && level == 0)
        {
            std::size_t found = line.find("#version");
            title = found != std::string::npos;
#ifdef _GLES_
            line = "#version 300 es";
#endif
            find = true;
        }
        
        if (std::regex_search(line, matches, re))
        {
            std::string include_file = matches[1];
            if(std::find(headers.begin(), headers.end(), include_file)==headers.end())
            {
                headers.push_back(include_file);
#ifdef _GLES_
                size_t idx = include_file.rfind("/");
                include_file = include_file.replace(0, idx+1, "");
#endif
                std::string include_string = openFile(include_file.c_str());
                output << preprocess(include_string, "", level + 1) << std::endl;
            }
        }
        else
        {
            output <<  line << std::endl;
        }
        if(title)
        {
#ifdef _GLES_
            output<<"precision mediump float;"<<std::endl;
#endif
            output<<macro<<std::endl;
        }
        ++line_number;
    }
    return output.str();
}

//complete shader for debug
void Shader::save(std::string text, const char* name)
{
    std::string path(name);
#ifdef _GLES_
    path = "/tmp/"+path;
#else
    path = "temp/"+path;
#endif
#ifndef TARGET_IPHONE_SIMULATOR
    return;
#endif
    std::ofstream file;
    file.exceptions (std::ofstream::failbit | std::ofstream::badbit);
    try
    {
        file.open(path,std::ostream::trunc);
        file << text;
        file.close();
    }
    catch (std::ofstream::failure e)
    {
        std::cout << "ERROR::SHADER::SAVE, " <<name<< std::endl;
    }
}

std::string Shader::openFile(const char* name)
{
    std::string text;
    std::ifstream file;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
#ifdef _GLES_
        std::string path = getPath(name);
#else
        std::string path(name);
        path = "shader/" + path;
        path = WORKDIR + path;
#endif
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        text = stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::READ, "<<name << std::endl;
    }
    return text;
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
