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
    UIEvent()
    {
        std::cout<<"uievent called: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
    }
    
    virtual bool IsTarget(float x,float y) { return false;}
    
    virtual void OnTriger() {}
};

#endif /* uievent_h */
