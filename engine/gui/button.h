//
//  button.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef button_h
#define button_h

#include <glm/glm.hpp>
#include "uibase.h"
#include "uievent.h"
#include "eventmgr.h"
#include "uimgr.h"
#include "ttfont.h"
#include "label.h"

class UIButton : public UILabel
{
public:
        
    UIButton(const glm::vec2 pos,const glm::vec3 color = glm::vec3(1), const float scale = 1.0f,
           const std::string text="", const int evtid = 0,const bool interact=true);
    
    ~UIButton();
    
    void Draw();
    
private:
    
    void DrawBackground();
    
    void InitBackground();
    
protected:
    unsigned int vbo, vao;
    Shader* shader;
    
};

#endif /* button_h */

