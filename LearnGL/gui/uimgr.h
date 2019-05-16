//
//  uimgr.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef uimgr_h
#define uimgr_h

#include <vector>
#include "uibase.h"

class UIManager
{
public:
    static UIManager* getInstance()
    {
        return &instance;
    }

    void Regist(UIBase* ui)
    {
        vector.push_back(ui);
    }
    
    void Remove(UIBase* ui)
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
    
    void Draw()
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
    
private:
    UIManager() { }
    ~UIManager() { }
    UIManager(const UIManager&);
    UIManager& operator=(const UIManager &);
    static UIManager instance;
    
    std::vector<UIBase*> vector;
};


#endif /* uimgr_h */
