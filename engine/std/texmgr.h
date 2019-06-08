//
//  texmgr_h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef texmgr_h
#define texmgr_h

#include "texture.h"
#include "asset.h"


using namespace std;


namespace engine
{

/*
 texture manager
 store all texture loaded, to avoid load repeat
 */
    
    
    
    class TexMgr
    {
        
        DeclareSington(TexMgr)
        
                
    public:
    
        bool FindTexture(GLuint& texid, size_t& idx);
        
        bool FindTexture(string& texture, GLuint& texid, size_t& idx);
        
        void RealRemove(size_t idx);
        
        void LoadTex(std::string& texture, EXT ext, GLuint& texid);
        
        bool RemvTexture(GLuint texid);
        
        bool RemvTexture(std::string& texture);
        
        void UnloadAllTexture();
        
    private:
        vector<string> _textures;
        vector<GLuint> _texids;
        vector<size_t> _references;
        size_t _num_tex;
        
    };
    

}
#endif /* texmgr_h */
