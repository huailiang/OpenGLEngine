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
#include "uibase.h"
#include "uievent.h"
#include "eventmgr.h"

class Label : public UIBase, public UIEvent
{
    
public:
    Label(const char* text)
    {
        Label(false,text);
    }

    Label(bool interact, const char* text)
    {
        this->interact = interact;
        this->text = text;
        if(interact)
        {
            EventMgr::getInstance()->RegistEvt(*this);
        }
    }
    
    
    void Draw()
    {
        
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
    const char* text;
};

#endif /* label_h */
