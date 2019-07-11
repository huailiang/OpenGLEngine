//
//  EventMgr.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef EventMgr_h
#define EventMgr_h

#include "../engine.h"
#include "uievent.h"


using namespace std;

namespace engine
{

    class EventMgr
    {
        
        DeclareSington(EventMgr)
        
        
    public:
        
        void RegistEvt(UIEvent* evt);
        
        void RemoveEvt(UIEvent* evt);
        
        void Triger(float x, float y, int action);
        
        void DoTriger(float x, float y);
        
    private:
        std::vector<UIEvent*> ui_events;
    };

}
#endif /* EventMgr_h */
