//
//  label.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef label_h
#define label_h

#include <glm/glm.hpp>
#include <string>
#include "uibase.h"
#include "uievent.h"
#include "eventmgr.h"
#include "uimgr.h"
#include "ttfont.h"


class Label : public UIBase, public UIEvent
{
public:
    Label(){ std::cout<<"warn: called default constructor of label"<<std::endl; }
    
    Label(const glm::vec2 pos,const glm::vec3 color=glm::vec3(1), const float scale = 1.0f, const std::string text="", const bool interact=true)
    {
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
    
    ~Label()
    {
        if(interact)
        {
            EventMgr::getInstance()->RemoveEvt(this);
        }
        UIManager::getInstance()->Remove(this);
    }
    
    void Draw()
    {
        TTFont* font = TTFont::getInstance();
        len = font->RenderText(text, posx, posy, scales, color);
    }
    
    bool IsHit(float x, float y)
    {
        float xrg = len * 0.5f;
        float yrg = FONT_SIZE * 0.5f;
        bool hit = abs(x-getCenterX())< xrg && abs(SCR_HEIGHT - y - posy)< yrg;
        return hit;
    }
    
    void OnTriger()
    {
        cout<<"interact triger: "<<text<<endl;
    }
    
    void setText(const std::string text)
    {
        this->text = text;
    }
    
    float getCenterX()
    {
        return len * 0.5f + posx;
    }
    
private:
    glm::vec3 color;
    std::string text;
    float len;
    float scales;
};

#endif /* label_h */
