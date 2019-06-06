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
#include "common.h"

using namespace std;



namespace engine
{

/*
 texture manager
 store all texture loaded, to avoid load repeat
 */

    extern vector<string> _textures;
    extern vector<GLuint> _texids;
    extern vector<size_t> _references;
    extern size_t _num_tex;
    
    bool FindTexture(string& texture, GLuint& texid);
    
    void LoadTexture(std::string path, GLuint& texID);
    
    bool RemvTexture(GLuint texid);
    
    bool RemvTexture(std::string& texture);
    
    /*
     called when scene change
     */
    void UnloadAllTexture();

}
#endif /* texmgr_h */
