#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"

class Skybox
{
public:
    Skybox(Camera* camera,std::string name);
    ~Skybox();
    void Draw();

private:
    void init_tex();
    void init_buff();
    unsigned int loadCubemap(std::vector<std::string> faces);

private:
    unsigned int vao, vbo;
        std::vector<std::string> faces;
        unsigned int cubemapTexture;
        Shader* shader;
        Camera* camera;
        std::string name;
};

#endif // SKYBOX_H
