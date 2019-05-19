//
//  label.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/15/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef label_h
#define label_h

#include <glm/glm.hpp>
#include <string>
#include "uibase.h"
#include "uievent.h"
#include "eventmgr.h"
#include "uimgr.h"
#include "ttfont.h"


class Label : public UIBase, public UIEvent
{
public:
    Label(){ std::cout<<"warn: called default constructor of label"<<std::endl; }
    
    Label(const glm::vec2 pos,const glm::vec3 color = glm::vec3(1), const float scale = 1.0f,
          const std::string text="", const int evtid = 0,const bool interact=true)
    {
        UIEvent::evtid = evtid;
        this->scales = scale;
        this->interact = interact;
        this->text = text;
        this->color = color;
        SetPos(pos.x, pos.y);
        if(interact)
        {
            EventMgr::getInstance()->RegistEvt(this);
        }
        UIManager::getInstance()->Regist(this);
        InitBackground();
        Draw();
    }
    
    ~Label()
    {
        if(interact)
        {
            EventMgr::getInstance()->RemoveEvt(this);
        }
        UIManager::getInstance()->Remove(this);
        TTFont::getInstance()->RenderText("", 0, 0, 0, glm::vec3(0));
        Delete();
    }
    
    void Draw()
    {
        DrawBackground();
        TTFont* font = TTFont::getInstance();
        len = font->RenderText(text, posx, posy, scales, color);
    }
    
    bool IsHit(float x, float y)
    {
        float xr = len * 0.5f;
        float yr = FONT_SIZE * 0.5f;
        bool hit = abs(x-getCenterX()) < xr && abs(SCR_HEIGHT - y - posy) < yr;
        return hit;
    }
    
    void OnTriger()
    {
      /*  cout<<"interact triger: "<<text<<endl;*/
    }
    
    void setText(const std::string text)
    {
        this->text = text;
    }
    
    float getCenterX()
    {
        return len * 0.5f + posx;
    }
    
    std::string getText()
    {
        return this->text;
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
        shader = new Shader("shader/button.vs","shader/button.fs","shader/button.gs");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glBindVertexArray(0);
    }
    
    void Delete()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        delete shader;
        shader = NULL;
    }
    
private:
    std::string text;
    glm::vec3 color;
    float len;
    float scales;
    
    unsigned int vbo, vao;
    Shader* shader;
};

#endif /* label_h */
