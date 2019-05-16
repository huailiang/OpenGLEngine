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
#include "ttfont.h"


class Label : public UIBase, public UIEvent
{
    
public:
    Label(){ std::cout<<"warn: called default constructor of label"<<std::endl; }
    
    Label(std::string text,const glm::vec2 pos, const glm::vec3 color)
    {
        Label(text,pos,color,1.0f);
    }
    
    Label(std::string text,const glm::vec2 pos,const glm::vec3 color,const float scale)
    {
        Label(text,pos,color,scale,false);
    }
    
    Label(std::string text,const glm::vec2 pos,const glm::vec3 color,const float scale,const bool interact)
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
        Draw();
    }
    
    void Draw()
    {
        TTFont* font = TTFont::getInstance();
        font->RenderText(text, posx, posy, scales, color);
    }
    
    bool IsTarget(float x, float y)
    {
        return false;
    }
    
    void OnTriger()
    {
        
    }
    
private:
    glm::vec3 color;
    std::string text;
    float scales;
};

#endif /* label_h */
