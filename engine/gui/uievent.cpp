//
//  uievent.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "uievent.hpp"

namespace engine
{

    void UIEvent::RegistCallback(ClickCallback cb)
    {
        RegistCallback(cb, NULL);
    }

    void UIEvent::RegistCallback(ClickCallback cb, void* arg)
    {
        this->callback = cb;
        this->arg = arg;
    }

    void UIEvent::Dispacher()
    {
        if(callback)
        {
            callback(this, arg);
        }
    }

}
