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
#include "uibase.hpp"
#include "uiobject.hpp"
#include "uimgr.hpp"
#include "ttfont.hpp"
#include "label.hpp"

namespace engine
{

    class UIButton : public UILabel
    {
        
    public:
            
        UIButton(const glm::vec2 pos,const glm::vec3 color = glm::vec3(1), const float scale = 1.0f,
               const std::string text="", const int uid = 0);
        
        virtual ~UIButton();
        
        void Draw();
        
    private:
        
        void DrawBackground();
        
        void InitBackground();
        
    protected:
        GLuint vbo, vao;
        Shader* shader = nullptr;
        
    };

}
#endif /* button_h */

