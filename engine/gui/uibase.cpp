//
//  uibase.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "uibase.h"



UIBase::UIBase(float x, float y): posx(x), posy(y)
{
}


UIBase::UIBase(const glm::vec2 pos): posx(pos.x), posy(pos.y)
{   
}

void UIBase::SetPos(float x, float y)
{
    this->posx = x;
    this->posy = y;
}
