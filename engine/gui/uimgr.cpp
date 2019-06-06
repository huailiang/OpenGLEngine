//
//  uimgr.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/6/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "uimgr.h"


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
    
}
