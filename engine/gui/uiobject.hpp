//
//  uiobject.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef uiobject_h
#define uiobject_h

#include <iostream>

namespace engine
{
    
    class UIObject
    {
        
    public:
        
        int uid;
        
        UIObject(int uid) : uid(uid) { }

        virtual ~UIObject() { }
    };
    
    #define ClickCallback std::function<void (UIObject*)>
    
    #define Bindfunc(f) std::bind(&f, this, std::placeholders::_1)

}
#endif /* uiobject_h */
