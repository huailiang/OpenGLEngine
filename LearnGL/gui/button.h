//
//  button.h
//  LearnGL
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

class Button : public Label
{
public:
    
    Button(){ std::cout<<"warn: called default constructor of button"<<std::endl; }
    
    Button(const glm::vec2 pos,const glm::vec3 color = glm::vec3(1), const float scale = 1.0f,
           const std::string text="", const int evtid = 0,const bool interact=true):
        Label(pos,color,scale,text, evtid, interact)
    {
        InitBackground();        
    }

    ~Button()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        delete shader;
        shader = NULL;
    }
    
    void Draw()
    {
        DrawBackground();
        Label::Draw();
    }
    
private:
    
    void DrawBackground()
    {
        float x = getCenterX()/SCR_WIDTH;
        float y = posy / SCR_HEIGHT;
        float w = 2.5 * len / (float)SCR_WIDTH;
        float h = 2.4 * FONT_SIZE * scales/ (float)SCR_HEIGHT;
        shader->use();
        shader->setVec4("aArg", x, y+ 0.12*h, w, h);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    }
    
    void InitBackground()
    {
        float vertices[] = {0.0f,  0.0f};
        shader = new Shader("shader/gizmos/button.vs","shader/gizmos/pixel.fs","shader/gizmos/button.gs");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glBindVertexArray(0);
    }
    
protected:
    unsigned int vbo, vao;
    Shader* shader;
    
};

#endif /* button_h */
