#include "scenewrap.h"
#include "ui_enginewidget.h"
#include "../engine/std/texture.h"
#include "../engine/std/asset.h"
#include "../engine/gui/ttfont.h"

SceneMgr SceneMgr::instance;

float lastTime = 0;


SceneWrap::SceneWrap()
{

}

SceneWrap::~SceneWrap()
{
    SceneMgr::getInstance()->LeaveScene();
    OnApplicationQuit();
}


void SceneWrap::OnClickGL(int x, int y)
{
    EventMgr::getInstance()->Triger((float)x, (float)y, GLFW_RELEASE);
}

void SceneWrap::HandleClick(int evtid)
{
    std::cout<<"evt id:"<<evtid<<std::endl;
}


void SceneWrap::KeyBoard(int key)
{
    SceneMgr::getInstance()->ProcessKeyboard(key, deltatime);
}

void SceneWrap::initialize()
{
    SystemInfo();
    TTFont::getInstance()->initial();
    SceneMgr::getInstance()->Init();
}

void SceneWrap::Draw()
{
      float timeValue = GetRuntime();
      deltatime  = timeValue-lastTime;
      lastTime= timeValue;
      SceneMgr::getInstance()->Draw(deltatime);
      UIManager::getInstance()->Draw();
}

