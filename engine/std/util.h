//
//  tool.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/08/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef tool_h
#define tool_h


#include "../common.h"
#include "material.h"
#include "texmgr.h"

namespace engine
{
    
    using namespace std;
    
    extern map<string,GLuint> loaded_textures;
    
    extern string curr_obj;
    
    int CheckDir(const char* path);
    
    bool LoadObj(std::string name);

    MeshData* ReadMesh(std::string name);
    
    void ReadObjMaterial(std::string name, ObjMaterial* mat);
    
    void ReadSummary(const std::string name, vector<string>& items);
        
}

#endif /* tool_h */
