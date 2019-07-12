//
//  eventmgr.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/6/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "eventmgr.h"

namespace engine
{
    
    EventMgr EventMgr::instance;
    
    void EventMgr::RegistEvt(UIEvent* evt)
    {
        ui_events.push_back(evt);
    }
    

    void EventMgr::RemoveEvt(UIEvent* evt)
    {
        for(vector<UIEvent*>::iterator iter=ui_events.begin(); iter!=ui_events.end();iter++)
        {
            if(*iter == evt)
            {
                ui_events.erase(iter);
                break;
            }
        }
    }

    
    void EventMgr::Triger(float x, float y, int action)
    {
#ifndef _GLES_
        float scale = RENDER_WIDTH / (SCR_WIDTH * 2.0f);
        float xx = x / scale;
        scale = RENDER_HEIGTH / (SCR_HEIGHT * 2.0f);
        float yy = y / scale;
        if(action == GLFW_RELEASE) DoTriger(xx, yy, action);
#endif
    }
    
    void EventMgr::DoTriger(float x, float y)
    {
        for (size_t i=0; i<ui_events.size(); i++)
        {
            UIEvent* et = ui_events[i];
            if(et->IsHit(x, y))
            {
                et->OnTriger();
                et->Dispacher();
            }
        }
    }
    

}
