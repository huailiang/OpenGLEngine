//
//  button.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "button.h"


Button::Button(const glm::vec2 pos,const glm::vec3 color, const float scale,const std::string text, const int evtid,const bool interact):
Label(pos,color,scale,text, evtid, interact)
{
    InitBackground();
}


Button::~Button()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    delete shader;
    shader = NULL;
}


void Button::Draw()
{
    DrawBackground();
    Label::Draw();
}

void Button::DrawBackground()
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

void Button::InitBackground()
{
    shader = new Shader("gizmos/button.vs","gizmos/pixel.fs","gizmos/button.gs");
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zeroVertices), &zeroVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBindVertexArray(0);
}
