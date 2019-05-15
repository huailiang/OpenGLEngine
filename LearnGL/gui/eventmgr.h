//
//  EventMgr.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef EventMgr_h
#define EventMgr_h

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "uievent.h"

class EventMgr
{
public:
    
    static EventMgr* getInstance()
    {
        return &instance;
    }
    
    void RegistEvt(UIEvent evt)
    {
        ui_events.push_back(evt);
    }
    
    void Triger(float x, float y)
    {
        size_t size = ui_events.size();
        for (size_t i=0; i<size; i++)
        {
            if(ui_events[i].IsTarget(x, y))
            {
                ui_events[i].OnTriger();
            }
        }
    }
    
private:
    EventMgr() { std::cout<<"EventMgr created!"<<std::endl; }
    ~EventMgr() { std::cout<<"EventMgr destroyed!"<<std::endl; }
    EventMgr(const EventMgr &);
    EventMgr& operator=(const EventMgr &);
    static EventMgr instance;
    
    std::vector<UIEvent> ui_events;
};


#endif /* EventMgr_h */
