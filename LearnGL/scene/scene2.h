//
//  scene2.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene2_h
#define scene2_h

#include "scene.h"

class Scene2 : public Scene
{
    
public:
    
    SceneType getType()
    {
        return TY_Scene2;
    }
    
    void InitLight()
    {
        
    }
    
    void InitScene()
    {
        
    }

    
    void DrawUI()
    {
        Scene::DrawUI();
    }
    
    void DrawTerrain()
    {
        
    }
    
    void DrawChar()
    {
        
    }
};


#endif /* scene2_h */
