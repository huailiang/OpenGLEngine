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

typedef void (*ClickCallback)(UIEvent*,void*);

class UIEvent
{
public:
    int evtid;
    
    UIEvent() { }
    
    UIEvent(const UIEvent& other)
    {
        std::cout<<"copy constructor called"<<std::endl;
    }
    
    virtual ~UIEvent() { }
    
    virtual bool IsHit(float x,float y) = 0;
    
    virtual void OnTriger() { } 
    
    void RegistCallback(ClickCallback cb)
    {
        RegistCallback(cb, NULL);
    }
    
    void RegistCallback(ClickCallback cb, void* arg)
    {
        this->callback = cb;
        this->arg = arg;
    }
    
    void Dispacher()
    {
        if(callback)
        {
            callback(this, arg);
        }
    }

protected:
    bool interact;

private:
    ClickCallback callback;
    void* arg;

};

#endif /* uievent_h */
