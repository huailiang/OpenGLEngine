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
#include "animation.h"

namespace engine
{
    
    using namespace std;
    
    #define MODEL_XML 0x01
    #define MODEL_OBJ 0x02
    #define MODEL_FBX 0x04
    #define MODEL_ANI 0x10 
    
    typedef unsigned int MODEL_TYPE;

    
    extern map<string,GLuint> loaded_textures;
    
    extern string curr_obj;
    
    int CheckDir(const char* path);
    
    bool LoadObj(const std::string name);
    
    void WriteMesh(const std::string name,vector<int>& indices, vector<Vertex>& vertices, VertType type, std::string dir="");
    
    MeshData* ReadMesh(const std::string name, const std::string objdir = "", const short ilod = 0);
    
    void RecalcuteLod(MeshData* data, const std::string name, const std::string objdir, const short ilod);
    
    void WriteMaterial(const string name,const std::string texture[],std::string dir="");
    
    void ReadObjMaterial(const std::string name, ObjMaterial* mat);
    
    MODEL_TYPE ReadSummary(const std::string name, vector<string>& items);
    
    void ReadSkeleton(Skeleton* skeleton, const std::string name, const std::string dir);
    
    // 加载xml 模型
    void LoadXmlObj(const char* file);
    
    // 加载骨骼动画
    void LoadXmlSkeleton(const char* file);
    
    // 计算三角面所在的切线、副切线
    void caltangent(const Vertex* v1, const Vertex* v2, const Vertex* v3, glm::vec3* tan, glm::vec3* bit);
    
    void caltangent(const BaseVert3* v1, const BaseVert3* v2, const BaseVert3* v3, glm::vec3* tan, glm::vec3* bit);
    
    void caltangent(const ColorVertex* v1, const ColorVertex* v2, const ColorVertex* v3, glm::vec3* tan, glm::vec3* bit);
    
    void caltangent(const CompxVertex* v1, const CompxVertex* v2, const CompxVertex* v3, glm::vec3* tan, glm::vec3* bit);
    
    // 顶点转换
    void ConvertVertex(Vertex* vert, TangVertex* tanVert);
    
    void ConvertVertex(CompxVertex* vert, TangVertex* tanVert);
}

#endif /* tool_h */
