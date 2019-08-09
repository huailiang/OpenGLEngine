//
//  uibase.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "uibase.hpp"

namespace engine
{

    UIBase::UIBase(float x, float y, const int uid) : posx(x), posy(y), UIObject(uid)
    {
    }


    UIBase::UIBase(const glm::vec2 pos, const int uid) : posx(pos.x), posy(pos.y),  UIObject(uid)
    {
    }

    void UIBase::SetPos(float x, float y)
    {
        this->posx = x;
        this->posy = y;
    }

    void UIBase::Dispacher()
    {
        if(callback)
        {
            (callback)(this);
        }
    }
    
    void UIBase::RegistCallback(ClickCallback callback)
    {
        this->callback = callback;
    }
    

}
