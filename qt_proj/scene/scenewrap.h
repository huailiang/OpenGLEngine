#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLFunctions_3_3_Core>
#include <qtimer.h>
#include <iostream>
#include "scenemgr.h"

using namespace engine;

class SceneWrap
{

public:

    SceneWrap();

    virtual ~SceneWrap();

    void InitEngine();

    void HandleClick(int evtid);

    void initialize();

    void Draw();

    void OnClickGL(int x, int y);

    void KeyBoard(int key);


private:
    int m_timerId;
};


#endif // SCENE_H
