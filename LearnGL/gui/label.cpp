//
//  label.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "label.h"

Label::Label(const glm::vec2 pos) :UIBase(pos)
{
    std::cout<<"warn: called default constructor of label"<<std::endl;
}

Label::Label(const glm::vec2 pos,const glm::vec3 color, const float scale, const std::string text, const int evtid,const bool interact)
    :UIBase(pos)
{
    UIEvent::evtid = evtid;
    this->scales = scale;
    this->interact = interact;
    this->text = text;
    this->color = color;
    SetPos(pos.x, pos.y);
    if(interact)
    {
        EventMgr::getInstance()->RegistEvt(this);
    }
    UIManager::getInstance()->Regist(this);
    Draw();
}

Label::~Label()
{
    if(interact)
    {
        EventMgr::getInstance()->RemoveEvt(this);
    }
    UIManager::getInstance()->Remove(this);
    TTFont::getInstance()->RenderText("", 0, 0, 0, glm::vec3(0));
}

void Label::Draw()
{
    TTFont* font = TTFont::getInstance();
    len = font->RenderText(text, posx, posy, scales, color);
}

bool Label::IsHit(float x, float y)
{
    float xr = len * 0.5f;
    float yr = FONT_SIZE * 0.5f;
    bool hit = abs(x-getCenterX()) < xr && abs(SCR_HEIGHT - y - posy) < yr;
    return hit;
}

void Label::OnTriger()
{
    /*  cout<<"interact triger: "<<text<<endl;*/
}

void Label::setText(const std::string text)
{
    this->text = text;
}

float Label::getCenterX()
{
    return len * 0.5f + posx;
}

std::string Label::getText()
{
    return this->text;
}
