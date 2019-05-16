//
//  button.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef button_h
#define button_h

#include <glm/glm.hpp>
#include "uibase.h"
#include "uievent.h"
#include "eventmgr.h"
#include "uimgr.h"

class Button : public UIBase, public UIEvent
{
public:
    
    void Draw()
    {
        
    }
    
    bool IsHit(float x, float y)
    {
        return false;
    }
    
    void OnTriger()
    {
        
    }
    
    Button()
    {
        UIManager::getInstance()->Regist(this);
    }
    
    ~Button()
    {
        if(interact)
        {
            EventMgr::getInstance()->RemoveEvt(this);
        }
        UIManager::getInstance()->Remove(this);
    }
    
};

#endif /* button_h */
