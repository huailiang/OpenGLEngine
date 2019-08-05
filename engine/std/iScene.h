//
//  iScene.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 8/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef iScene_h
#define iScene_h



namespace engine
{

    class Camera;
    
    class iScene
    {
    public:
        
        virtual bool drawShadow() = 0;
        
        virtual Camera* getCamera() = 0;
        
    };
    
    class iSceneMgr
    {
        virtual void setEngineScene() = 0;
    };
    
}

#endif /* iScene_h */
