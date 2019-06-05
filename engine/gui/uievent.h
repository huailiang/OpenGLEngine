//
//  uievent.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef uievent_h
#define uievent_h

#include <iostream>

namespace engine
{

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
    
    virtual ~UIEvent()
    {
        callback = NULL;
    }
    
    virtual bool IsHit(float x,float y) = 0;
    
    virtual void OnTriger() { } 
    
    void RegistCallback(ClickCallback cb);
    
    void RegistCallback(ClickCallback cb, void* arg);
    
    void Dispacher();
    
protected:
    bool interact;

private:
    ClickCallback callback = NULL;
    void* arg;

};

}
#endif /* uievent_h */
