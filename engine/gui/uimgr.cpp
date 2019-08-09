//
//  uimgr.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/6/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "uimgr.hpp"


namespace engine
{
    
    UIManager UIManager::instance;
    
    
    void UIManager::Regist(UIBase* ui)
    {
        vector.push_back(ui);
    }

    void UIManager::Remove(UIBase* ui)
    {
        for(std::vector<UIBase*>::iterator iter = vector.begin();iter!=vector.end();iter++)
        {
            if(*iter == ui)
            {
                vector.erase(iter);
                break;
            }
        }
    }


    void UIManager::Draw()
    {
        for(size_t i=0;i<vector.size();i++)
        {
            UIBase* ui = vector[i];
            if(ui)
            {
                ui->Draw();
            }
        }
    }
    
    void UIManager::Triger(float x, float y, int action)
    {
#ifndef _GLES_
        float scale = RENDER_WIDTH / (SCR_WIDTH * 2.0f);
        float xx = x / scale;
        scale = RENDER_HEIGTH / (SCR_HEIGHT * 2.0f);
        float yy = y / scale;
        if(action == GLFW_RELEASE) DoTriger(xx, yy);
#endif
    }
    
    void UIManager::DoTriger(float x, float y)
    {
        for (size_t i=0; i<vector.size(); i++)
        {
            auto it = vector[i];
            if(it->IsHit(x, y))
            {
                it->Dispacher();
            }
        }
    }
    
    
}
