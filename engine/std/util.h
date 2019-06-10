//
//  tool.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/08/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef tool_h
#define tool_h


#include "../engine.h"
#include "material.h"
#include "texmgr.h"

namespace engine
{
    
    using namespace std;
    
    extern map<string,GLuint> loaded_textures;
    
    extern string curr_obj;
    
    int CheckDir(const char* path);
    
    bool LoadObj(const std::string name);
    
    MeshData* ReadMesh(const std::string name, const std::string objdir = "");
    
    void ReadObjMaterial(const std::string name, ObjMaterial* mat);
    
    void ReadSummary(const std::string name, vector<string>& items);
    
    // 计算三角面所在的切线、副切线
    void caltangent(const Vertex* v1, const Vertex* v2, const Vertex* v3, glm::vec3* tan, glm::vec3* bit);
    
    void caltangent(const BaseVert3* v1, const BaseVert3* v2, const BaseVert3* v3, glm::vec3* tan, glm::vec3* bit);
    
    void caltangent(const ColorVertex* v1, const ColorVertex* v2, const ColorVertex* v3, glm::vec3* tan, glm::vec3* bit);
    
    void caltangent(const CompxVertex* v1, const CompxVertex* v2, const CompxVertex* v3, glm::vec3* tan, glm::vec3* bit);
    
    
}

#endif /* tool_h */
