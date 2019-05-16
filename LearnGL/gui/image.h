//
//  image.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef image_h
#define image_h

#include <glm/glm.hpp>
#include "uibase.h"
#include "uievent.h"
#include "eventmgr.h"
#include "uimgr.h"

class Image : public UIBase, public UIEvent
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
    
    Image()
    {
        UIManager::getInstance()->Regist(this);
    }
    
    ~Image()
    {
        if(interact)
        {
            EventMgr::getInstance()->RemoveEvt(this);
        }
        UIManager::getInstance()->Remove(this);
    }
    
};


#endif /* image_h */
