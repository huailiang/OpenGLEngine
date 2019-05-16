//
//  uievent.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef uievent_h
#define uievent_h

#include <iostream>

class UIEvent
{
public:
    UIEvent() { }
    
    UIEvent(const UIEvent& other)
    {
        std::cout<<"copy constructor called"<<std::endl;
    }
    
    virtual bool IsTarget(float x,float y) = 0;
    
    virtual void OnTriger() = 0 ;
};

#endif /* uievent_h */
