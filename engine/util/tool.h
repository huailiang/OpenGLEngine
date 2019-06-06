//
//  tool.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/08/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef tool_h
#define tool_h



#include "common.h"
#include "material.h"

namespace engine
{
    
    using namespace std;
    
    extern map<string,GLuint> loaded_textures;
    
    extern string curr_obj;
    
    bool LoadObj(std::string name);
    
    void WriteMesh(std::string name,vector<int>& indices, vector<Vertex>& vertices,std::string texture[]);

    MeshData* ReadMesh(std::string name);
    
    void ReadSummary(const std::string name, vector<string>& items);
        
}

#endif /* tool_h */
