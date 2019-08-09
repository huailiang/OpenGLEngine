//
//  uimgr.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef uimgr_h
#define uimgr_h


#include "uibase.hpp"
#include "../engine.h"


namespace engine
{

    class UIManager
    {
        
        DeclareSington(UIManager);
        
    public:
        
        void Regist(UIBase* ui);
        
        void Remove(UIBase* ui);
        
        void Draw();
        
        void Triger(float x, float y, int action);
        
        void DoTriger(float x, float y);
        
    private:
        
        std::vector<UIBase*> vector;
    };

}

#endif /* uimgr_h */
