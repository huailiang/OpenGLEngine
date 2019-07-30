//
//  label.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef label_h
#define label_h

#include <glm/glm.hpp>
#include <string>
#include "uibase.hpp"
#include "uievent.hpp"
#include "eventmgr.hpp"
#include "uimgr.hpp"
#include "ttfont.hpp"

namespace engine
{


    class UILabel : public UIBase, public UIEvent
    {
    public:
        
        UILabel(const glm::vec2 pos);
        
        UILabel(const glm::vec2 pos,const glm::vec3 color = glm::vec3(1), const float scale = 1.0f,
              const std::string text="", const int evtid = 0,const bool interact=true);
        
        virtual ~UILabel();
        
        virtual void Draw();
        
        bool IsHit(float x, float y);
        
        virtual void OnTriger();
        
        void setText(const std::string text);

        void drawText(const std::string text);
        
        float getCenterX();
        
        std::string getText();
        
    protected:
        std::string text;
        glm::vec3 color;
        float len;
        float scales;
    };

}

#endif /* label_h */
