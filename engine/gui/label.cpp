//
//  label.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "label.hpp"

namespace engine
{
    
    UILabel::UILabel(const glm::vec2 pos, const int uid ) :UIBase(pos, uid)
    {
        std::cout<<"warn: called default constructor of label"<<std::endl;
    }

    UILabel::UILabel(const glm::vec2 pos,const glm::vec3 color, const float scale, const std::string text, const int uid)
        :UIBase(pos, uid)
    {
        this->scales = scale;
        this->color = color;
        SetPos(pos.x, pos.y);
        UIManager::getInstance()->Regist(this);
        drawText(text);
    }

    UILabel::~UILabel()
    {
        UIManager::getInstance()->Remove(this);
        TTFont::getInstance()->RenderText("", 0, 0, 0, glm::vec3(0));
    }

    void UILabel::Draw()
    {
        TTFont* font = TTFont::getInstance();
        len = font->RenderText(text, posx, posy, scales, color);
    }

    bool UILabel::IsHit(float x, float y)
    {
        float xr = len * 0.5f;
        float yr = FONT_SIZE * 0.5f;
        bool hit = abs(x-getCenterX()) < xr && abs(UI_HEIGHT - y - posy) < yr;
        return hit;
    }

    void UILabel::OnTriger()
    {
        /*  cout<<"interact triger: "<<text<<endl;*/
    }

    void UILabel::setText(const std::string text)
    {
        this->text = text;
    }

    void UILabel::drawText(const std::string text)
    {
        setText(text);
        Draw();
    }

    float UILabel::getCenterX()
    {
        return len * 0.5f + posx;
    }

    std::string UILabel::getText()
    {
        return this->text;
    }

}
