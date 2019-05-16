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

enum UIEventType
{
    Press,
    Pop,
    Click
};

class UIEvent;

typedef void (*ClickCallback)(UIEvent* contex);

class UIEvent
{
public:
    int evtid;
    
    UIEvent() { }
    
    UIEvent(const UIEvent& other)
    {
        std::cout<<"copy constructor called"<<std::endl;
    }
    
    virtual bool IsHit(float x,float y) = 0;
    
    virtual void OnTriger() { } 
    
    void RegistCallback(ClickCallback cb)
    {
        this->callback = cb;
    }
    
    void Dispacher()
    {
        if(callback)
        {
            callback(this);
        }
    }

private:
    ClickCallback callback;

};

#endif /* uievent_h */
