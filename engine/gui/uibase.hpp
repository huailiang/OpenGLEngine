//
//  UIBase.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef UIBase_h
#define UIBase_h

#include <glm/glm.hpp>
#include "uiobject.hpp"

namespace engine
{
    
    class UIBase : public UIObject
    {
    public:
        
        UIBase(float x, float y, const int uid );
        
        UIBase(const glm::vec2 pos, const int uid );
        
        virtual void Draw() = 0;
        
        void SetPos(float x, float y);
        
        virtual bool IsHit(float x,float y) = 0;
        
        void Dispacher();
        
        void RegistCallback(ClickCallback callback);
        
    protected:
        float posx, posy;
        ClickCallback callback;
        int iarg = 0;
    };
    
}

#endif /* UIBase_h */
