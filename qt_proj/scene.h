#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLFunctions_3_3_Core>
#include <qtimer.h>
#include <iostream>
#include "../engine/std/shader.h"
#include "../engine/std/camera.h"
#include "../engine/std/light.h"
#include "../engine/std/skybox.h"
#include "../engine/gui/label.h"

using namespace engine;


class Scene
{

public:
    Scene();
    virtual ~Scene();
    void HandleClick(int evtid);
    void initialize();
    void Draw();
    void InitShader();

private:
    unsigned int vbo, vao;
    LightShader* shader;
    MeshData* cube;
    Camera* camera;
    Light* light;
    unsigned int texture1, texture2;
    Skybox* skybox;
    int m_timerId;
    UILabel* label;
};

#endif // SCENE_H
