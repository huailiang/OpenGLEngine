//
//  UIBase.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef UIBase_h
#define UIBase_h

class UIBase
{
public:
    virtual void Draw() = 0;
    
    void SetPos(float x, float y)
    {
        this->posx = x;
        this->posy = y;
    }
    
protected:
    float posx, posy;
};

#endif /* UIBase_h */
