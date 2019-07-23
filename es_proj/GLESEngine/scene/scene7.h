//
//  scene7.h
//  GLESEngine
//
//  Created by 彭怀亮 on 7/22/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene7_h
#define scene7_h

#include "arscene.h"
#ifdef _GLES_
#include "IARInterface.h"
#endif

class Scene7 : public ARScene
{
    
public:
    
    ~Scene7()
    {
        SAFE_DELETE(pick);
        SAFE_DELETE(match);
        DELETE_TEXTURE(texID);
    }
    
    int getType() { return TY_Scene7; }
    
    void InitLight() { light = new DirectLight(vec3(1.0f), vec3(0.0f, 0.0f, -2.0f)); }
    
    virtual void InitScene()
    {
        ARScene::InitScene();
        glGenTextures(1, &m_backgroundTextureId);
        glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        Texture tex("textures/container", _JPG, &texID);
        RenderQuad(texID);
    }
    
    void DrawUI()
    {
        Scene::DrawUI();
        pick = new engine::UIButton(vec2(720, 360), vec3(1,1,0), 0.6f, "pick", 0);
        match = new  engine::UIButton(vec2(720, 330), vec3(1,1,0), 0.6f, "match", 1);
        pick->RegistCallback(OnClick, this);
        match->RegistCallback(OnClick, this);
    }
    
    void DrawScene()
    {
        DrawBackground();
    }
    
    
    void StartPick()
    {
        if(arPtr)
        {
            arPtr->GetAlbumPicker(OnPickerFinish, this);
        }
    }
    
    void OnPicker()
    {
        std::cout<<"pick finish: "<<std::endl;
        float width = 0, height = 0;
        void* data = arPtr->GetImageData(&width, &height);
        std::cout<<"width:"<<width<<" height:"<<height<<" data"<<data<<std::endl;
        glActiveTexture(GL_TEXTURE0);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    
    void StartMatch()
    {
        // 
    }
    
    void Process(IARInterface* ar)
    {
        arPtr = ar;
    }
    
private:
    
    static void OnPickerFinish(void* arg)
    {
        Scene7* sc = (Scene7*)arg;
        sc->OnPicker();
    }

    static void OnClick(engine::UIEvent* contex, void* arg)
    {
        Scene7* scene = (Scene7*)(arg);
        int evtid = contex->evtid;
        if(evtid == 0) scene->StartPick();
        if(evtid == 1) scene->StartMatch();
    }
    
private:
    engine::UIButton* pick, *match;
    IARInterface* arPtr = nullptr;
    GLuint texID;
};


#endif /* scene7_h */
