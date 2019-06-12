//
//  tool.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/08/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//


#include "util.h"


#include <memory>
#ifdef __APPLE__
#include <dirent.h>
#endif

#ifdef _GLES_
#include "FilePath.h"
#else
#include <stdio.h>
#include "../ext/xml/tinyxml.h"
#endif

#define TINYOBJLOADER_IMPLEMENTATION
#include "../ext/tiny_obj_loader.h"
#include "animation.h"

namespace engine
{
    string curr_obj;
    map<string,GLuint> loaded_textures;
    
    void WriteSummary(std::vector<tinyobj::shape_t>& shapes);
    
    int CheckDir(const char* path)
    {
#ifdef __APPLE__
#ifndef _GLES_
        if(opendir(path)==NULL)
        {
            char temp[200];
            memset(temp, '\0', sizeof(temp));
            strcat(temp, "mkdir -p ");
            strcat(temp, path);
            system(temp);
            return 1;//create success
        }
        else
        {
            return -1;//can not make a dir;
        }
#endif
#endif
        return 0;
    }
    
    void getTextures(tinyobj::mesh_t& mesh, std::vector<tinyobj::material_t>& materials,std::string* vec)
    {
        int id = mesh.material_ids[0];
        tinyobj::material_t mat = materials[id];
        vec[0] = mat.diffuse_texname;
        vec[1] = mat.normal_texname;
        vec[2] = mat.specular_texname;
        vec[3] = mat.ambient_texname;
    }
    
    size_t findVector(std::vector<int>& vect, int& key)
    {
        size_t len = vect.size();
        for (size_t i=0; i<len; i++) {
            if(vect[i]==key) return i;
        }
        return len;
    }
    
    
    void readtex(ifstream& f, GLuint& id)
    {
        string str = "";
        int ext = 0;
        f.read((char*)&ext,sizeof(int));
        if(ext > 0)
        {
            readstring(f, str);
        }
        if(!str.empty())
        {
            str = "objects/" + curr_obj +"/" + str;
            TexMgr::getInstance()->LoadTex(str,(EXT)ext, id);
        }
    }
    
    
    void writestring(ofstream& f, std::string& str)
    {
        size_t len = str.size();
        f.write((char*)&len, sizeof(size_t));
        if(len > 0) f.write(str.c_str(), len);
    }
    
    void writevec3(ofstream& f, vec3& v)
    {
        f.write((char*)&(v.x),sizeof(float));
        f.write((char*)&(v.y),sizeof(float));
        f.write((char*)&(v.z),sizeof(float));
    }
    
    void writevec4(ofstream& f, vec4& v)
    {
        f.write((char*)&(v.x),sizeof(float));
        f.write((char*)&(v.y),sizeof(float));
        f.write((char*)&(v.z),sizeof(float));
        f.write((char*)&(v.w),sizeof(float));
    }
    
    void writearray(ofstream& f, float arr[], int num)
    {
        loop0i(num) f.write((char*)(arr+i),sizeof(float));
    }
    
    void writemat4(ofstream& f, mat4 mat)
    {
        loop(4) loop0j(4) f.write((char*)&mat[i][(int)j],sizeof(float));
    }

    void WriteSummary(std::vector<tinyobj::shape_t>& shapes)
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            string basedir = "resources/engine/"+curr_obj+"/";
            CheckDir(basedir.c_str());
            ofs.open(basedir+"summary.sum",std::ofstream::binary | std::ios::out);
            unsigned int num = (unsigned int)shapes.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            for (size_t i =0; i<shapes.size(); i++) writestring(ofs, shapes[i].name);
            ofs.close();
        }
        catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MESH SUMMARY "<< std::endl;
        }
        vector<string> vec;
        ReadSummary(curr_obj, vec);
    }
    
    
    void WriteMaterial(const string name,const std::string texture[])
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            string basedir = "resources/engine/"+curr_obj+"/";
            ofs.open(basedir+name+".mat",std::ofstream::binary | std::ios::out);
            for(int i=0;i<TEXTURE_NUM;i++)
            {
                std::cout<<texture[i]<<std::endl;
                size_t idx = texture[i].rfind('.');
                if (idx > 1024) //invalid
                {
                    int ext =0;
                    ofs.write((char*)&ext,sizeof(int));
                }
                else
                {
                    std::string ex = texture[i].substr(idx);
                    int ext = getTextureFormat(ex.c_str());
                    ofs.write((char*)&ext,sizeof(int));
                    std::string name = texture[i].substr(0,idx);
                    writestring(ofs, name);
                }
            }
        }catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MATERIAL::SAVE, " <<name<< std::endl;
        }
    }
    
    void writeVert3(std::ofstream& ofs, const VertType type, const VertType t, float* ptr)
    {
        if((type & t) > 0)
        {
            ofs.write((char*)ptr++,sizeof(float));
            ofs.write((char*)ptr++,sizeof(float));
            ofs.write((char*)ptr++,sizeof(float));
        }
    }
    
    void WriteVertex(std::ofstream& ofs, Vertex* vert)
    {
        ofs.write((char*)&vert->Position.x,sizeof(float));
        ofs.write((char*)&vert->Position.y,sizeof(float));
        ofs.write((char*)&vert->Position.z,sizeof(float));
        ofs.write((char*)&vert->TexCoords.x,sizeof(float));
        ofs.write((char*)&vert->TexCoords.y,sizeof(float));
        ofs.write((char*)&vert->Normal.x,sizeof(float));
        ofs.write((char*)&vert->Normal.y,sizeof(float));
        ofs.write((char*)&vert->Normal.z,sizeof(float));
    }
    
    void WriteSkinVertex(std::ofstream& ofs, SkinVertex* vert)
    {
        Vertex* vertex = (Vertex*)vert;
        WriteVertex(ofs, vertex);
        ofs.write((char*)&vert->count,sizeof(int));
        for (int i=0; i<vert->count; i++) {
            ofs.write((char*)(vert->weight+i),sizeof(float));
            ofs.write((char*)(vert->boneindx+i),sizeof(int));
        }
    }
    
    void WriteMesh(const std::string name,vector<int>& indices, vector<Vertex>& vertices, VertType type, std::string dir)
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            if(dir.empty()) dir = curr_obj;
            string basedir = "resources/engine/"+dir+"/";
            ofs.open(basedir+name+".mesh",std::ofstream::binary | std::ios::out);
            unsigned int num = (unsigned int)indices.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            num = (unsigned int)vertices.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            ofs.write((char*)&type,sizeof(VertType));
            loop0i(indices.size()) ofs.write((char*)&indices[i],sizeof(unsigned int));
            loop0i(vertices.size()) WriteVertex(ofs, &vertices[i]);
            ofs.close();
        }
        catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MESH::SAVE, " <<name<< std::endl;
        }
    }
    
    
    void WriteMesh(const std::string name,vector<int>& indices, Vert* vertices, int num_vert, VertType type, std::string dir)
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            if(dir.empty()) dir = curr_obj;
            string basedir = "resources/engine/"+dir+"/";
            ofs.open(basedir+name+".mesh",std::ofstream::binary | std::ios::out);
            unsigned int num = (unsigned int)indices.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            ofs.write((char*)&num_vert,sizeof(unsigned int));
            ofs.write((char*)&type,sizeof(VertType));
            loop0i(indices.size())  ofs.write((char*)&indices[i],sizeof(unsigned int));
            for (size_t i=0; i<num_vert; i++)
            {
                if(type == 0x0111) WriteVertex(ofs, (Vertex*)(vertices + i));
                if(type == 0x2111) WriteSkinVertex(ofs, (SkinVertex*)(vertices + i));
            }
            ofs.close();
        }
        catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MESH::SAVE, " <<name<< std::endl;
        }
    }
    
    
    void ReadSummary(const std::string name, vector<string>& items)
    {
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            curr_obj = name;
            std::string path = getResPath("engine/"+name+"/summary.sum");
            ifs.open(path, std::ifstream::binary | std::ios::in);
            unsigned int num = 0;
            items.clear();
            ifs.seekg(0, ios::beg);
            ifs.read((char*)(&num), sizeof(unsigned int));
            string str;
            for (size_t i=0; i<num; i++) {
                readstring(ifs, str);
                items.push_back(str);
            }
        }
        catch (std::ifstream::failure e)
        {
            std::cerr<<"read summary error "<<name<<std::endl;
        }
    }

    bool LoadObj(const std::string name)
    {
        curr_obj = name;
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;
        std::string base_dir = "resources/objects/"+name+"/";
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (base_dir + name +".obj").c_str(), base_dir.c_str());
        if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
        if (!err.empty()) std::cerr << err << std::endl;
        WriteSummary(shapes);
        for (size_t s=0; s<shapes.size(); s++)
        {
            tinyobj::mesh_t mesh = shapes[s].mesh;
            vector<int> indices;
            vector<int> old_indices;
            vector<Vertex> vertices;
            size_t index_offset = 0;
            int indx_new = 0;
            for(size_t i = 0; i < mesh.num_face_vertices.size(); i++) {
                size_t num = mesh.num_face_vertices[i];
                Vertex vertex;
                for (size_t v = 0; v < num; v++) {
                    tinyobj::index_t idx = mesh.indices[index_offset + v];
                    size_t find = findVector(old_indices, idx.vertex_index);
                    if(find != old_indices.size())
                    {
                        indices.push_back((int)find);
                    }
                    else
                    {
                        vertex.Position.x = attrib.vertices[idx.vertex_index * 3 + 0];
                        vertex.Position.y = attrib.vertices[idx.vertex_index * 3 + 1];
                        vertex.Position.z = attrib.vertices[idx.vertex_index * 3 + 2];
                        vertex.TexCoords.x = attrib.texcoords[idx.texcoord_index * 2 + 0];
                        vertex.TexCoords.y = attrib.texcoords[idx.texcoord_index * 2 + 1];
                        vertex.Normal.x = attrib.normals[idx.normal_index * 3 + 0];
                        vertex.Normal.y = attrib.normals[idx.normal_index * 3 + 1];
                        vertex.Normal.z = attrib.normals[idx.normal_index * 3 + 2];
                        
                        old_indices.push_back(idx.vertex_index);
                        indices.push_back(indx_new++);
                        vertices.push_back(vertex);
                    }
                }
                index_offset += num;
            }
            std::string texture[TEXTURE_NUM];
            getTextures(mesh, materials,texture);
            WriteMesh(shapes[s].name, indices, vertices, 0x0111);
            WriteMaterial(shapes[s].name,texture);
            std::cout<<"export "<<shapes[s].name<<std::endl;
        }
        std::cout<<name<<" export finish "<<std::endl;
        return ret;
    }
    
    void ReadObjMaterial(const std::string name, ObjMaterial* mat)
    {
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            std::string path = getResPath("engine/"+curr_obj+"/"+name+".mat");
            ifs.open(path, std::ifstream::binary | std::ios::in);
            readtex(ifs, mat->diffuse_texture);
            readtex(ifs, mat->normal_texure);
            readtex(ifs, mat->specul_texture);
            readtex(ifs, mat->ambient_texture);
            if(mat->data==nullptr)
            {
                mat->data = ReadMesh(name);
            }
        }
        catch (std::ifstream::failure e)
        {
            std::cerr<<"read material error "<<name<<std::endl;
        }
    }
    
    MeshData* ReadMesh(const std::string name, const std::string objdir)
    {
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            if(!objdir.empty()) curr_obj = objdir;
            std::string path = getResPath("engine/"+curr_obj+"/"+name+".mesh");
            ifs.open(path, std::ifstream::binary | std::ios::in);
            unsigned int inds = 0,verts = 0, type = 0x0;
            ifs.seekg(0, ios::beg);
            ifs.read((char*)(&inds), sizeof(unsigned int));
            ifs.read((char*)(&verts), sizeof(unsigned int));
            ifs.read((char*)(&type), sizeof(unsigned int));
            MeshData* mesh = new MeshData();
            mesh->type = type;
            mesh->num_indice = inds;
            mesh->indices = new unsigned int[inds];
            mesh->num_vert = verts;
            mesh->vertices = new Vert*[verts];
            
            for (size_t i=0; i<inds; i++)
            {
                ifs.read((char*)(&mesh->indices[i]), sizeof(unsigned int));
            }
            for (size_t i=0; i<verts; i++)
            {
                switch (type) {
                    case 0x0111:
                    {
                        Vertex* vertex = new Vertex();
                        readv3(ifs, vertex->Position);
                        readv2(ifs, vertex->TexCoords);
                        readv3(ifs, vertex->Normal);
                        mesh->vertices[i] = vertex;
                    }
                    break;
                    case 0x0012:
                    {
                        BaseVert2* vert  = new BaseVert2();
                        readv2(ifs, vert->Position);
                        readv2(ifs, vert->TexCoords);
                        mesh->vertices[i] = vert;
                    }
                    break;
                    case 0x0011:
                    {
                        BaseVert3* vert  = new BaseVert3();
                        readv3(ifs, vert->Position);
                        readv2(ifs, vert->TexCoords);
                        mesh->vertices[i] = vert;
                    }
                    break;
                    case 0x1011:
                    {
                        ColorVertex* vert = new ColorVertex();
                        readv3(ifs, vert->Position);
                        readv2(ifs, vert->TexCoords);
                        readv3(ifs, vert->Color);
                        mesh->vertices[i] = vert;
                    }
                    break;
                    case 0x1111:
                    {
                        CompxVertex* vert = new CompxVertex();
                        readv3(ifs, vert->Position);
                        readv2(ifs, vert->TexCoords);
                        readv3(ifs, vert->Normal);
                        readv3(ifs, vert->Color);
                        mesh->vertices[i] = vert;
                    }
                    break;
                    case 0x2111:
                    {
                        SkinVertex* vert = new SkinVertex();
                        readv3(ifs, vert->Position);
                        readv2(ifs, vert->TexCoords);
                        readv3(ifs, vert->Normal);
                        ifs.read((char*)(&vert->count), sizeof(int));
                        for (size_t i=0; i<vert->count; i++) {
                            ifs.read((char*)(vert->weight+i), sizeof(float));
                            ifs.read((char*)(vert->boneindx+i), sizeof(int));
                        }
                        mesh->vertices[i] = vert;
                    }
                    break;
                    default:
                        std::cerr<<"vertex config not support format: 0x"<<hex<<type<<std::endl;
                        break;
                }
            }
            ifs.close();
            return mesh;
        }
        catch (std::ifstream::failure e)
        {
            std::cerr<<"read mesh error "<<name<<std::endl;
        }
        return nullptr;
    }
    
    
    void LoadXmlObj(const char* file)
    {
#ifndef _GLES_
        std::string path = "resources/xml/"+std::string(file)+".mesh.xml";
        FILE* fn= std::fopen(path.c_str(), "rb");
        if(!fn) std::cerr<<"xml mesh not found: " <<path <<std::endl;
        char line[1000],name[1000];
        memset(line, 0, 100);
        memset(name, 0, 100);
        std::vector<int> indices;
        std::vector<SkinVertex> vertices;
        int x,y,z;
        float f1,f2,f3;
        unsigned int normal_id = 0, texcoord_id = 0;
        while(fgets(line, 1000, fn ) != NULL)
        {
            if(sscanf(line," <face v1=\"%d\" v2=\"%d\" v3=\"%d\" />",&x,&y,&z) ==3)
            {
                indices.push_back(x); indices.push_back(y);indices.push_back(z);
            }
            if(sscanf(line," <position x=\"%f\" y=\"%f\" z=\"%f\" />",&f1,&f2,&f3) == 3)
            {
                SkinVertex a;
                a.Position=glm::vec3(f1,f2,f3);
                vertices.push_back(a);
            }
            if(sscanf(line," <normal x=\"%f\" y=\"%f\" z=\"%f\" />",&f1, &f2, &f3) == 3) vertices[normal_id++].Normal=vec3(f1,f2,f3);
            if(sscanf(line," <texcoord u=\"%f\" v=\"%f\" />",&f1, &f2 ) == 2)  vertices[texcoord_id++].TexCoords=vec2(f1,f2);
            if(sscanf(line," <vertexboneassignment vertexindex=\"%d\" boneindex=\"%d\" weight=\"%f\" />",&x, &y, &f1) == 3)
            {
                if(x>=vertices.size()) std::cerr<<"vertexboneassignment"<<x <<">="<<vertices.size()<<std::endl;
                int i=vertices[x].count;
                if(i < 3)
                {
                    vertices[x].weight[i] = f1;
                    vertices[x].boneindx[i] = y;
                    vertices[x].count++;
                }
            }
        }
        fclose(fn);
        
//        WriteMesh(fname, indices, vertices);
#endif
    }
    
    void LoadXmlSkeleton(const char* file)
    {
#ifndef _GLES_
        std::vector<Animation> animations;
        std::vector<Bone> bones;
        std::string path = "resources/xml/"+std::string(file)+".skeleton.xml";
        
        TiXmlDocument doc(path.c_str());
        if(!doc.LoadFile()) std::cerr<<path<< " load error "<<doc.ErrorDesc() <<std::endl;
        
        TiXmlNode* node = 0;
        TiXmlElement* skeletonNode = 0;
        TiXmlElement* animationsElement = 0;
        TiXmlElement* animationElement = 0;
        TiXmlElement* bonesElement = 0;
        TiXmlElement* boneElement = 0;
        TiXmlElement* boneHierarchyElement = 0;
        TiXmlElement* boneParentElement = 0;
        
        node = doc.FirstChild("skeleton");
        if(!node) std::cerr<<"node ptr 0"<<std::endl;
        skeletonNode = node->ToElement();
        if(!skeletonNode) std::cerr<<"skeletonNode ptr 0"<<std::endl;
        
        bonesElement = skeletonNode->FirstChildElement("bones"); assert(bonesElement);
        for(boneElement = bonesElement->FirstChildElement("bone"); boneElement; boneElement = boneElement->NextSiblingElement("bone"))
        {
            int result=0;
            const char* cBoneName;
            double dBoneID = 0,dPosX = 0,dPosY = 0,dPosZ = 0;
            double dAxisAngle = 0,dAxisX = 0,dAxisY = 0,dAxisZ = 0;
            
            result+= boneElement->QueryDoubleAttribute("id", &dBoneID);
            cBoneName = boneElement->Attribute("name");
            
            TiXmlElement *positionElement = 0,*rotateElement = 0,*axisElement = 0;
            positionElement = boneElement->FirstChildElement("position");assert( positionElement );
            rotateElement = boneElement->FirstChildElement("rotation");assert( rotateElement );
            axisElement = rotateElement->FirstChildElement("axis");assert( axisElement );
            result+= positionElement->QueryDoubleAttribute("x", &dPosX );
            result+= positionElement->QueryDoubleAttribute("y", &dPosY );
            result+= positionElement->QueryDoubleAttribute("z", &dPosZ );
            result+= rotateElement->QueryDoubleAttribute("angle", &dAxisAngle );
            result+= axisElement->QueryDoubleAttribute("x", &dAxisX );
            result+= axisElement->QueryDoubleAttribute("y", &dAxisY );
            result+= axisElement->QueryDoubleAttribute("z", &dAxisZ );
            assert( result == 0 );
            
            Bone bone;
            std::cout<<bone.name <<" "<< cBoneName<<std::endl;
            bone.nameLength = strnlen(bone.name,ANI_NAME_LEN);
            bone.rot[0]    = dAxisAngle;
            bone.rot[1]    = dAxisX;
            bone.rot[2]    = dAxisY;
            bone.rot[3]    = dAxisZ;
            bone.pos[0]    = dPosX;
            bone.pos[1]    = dPosY;
            bone.pos[2]    = dPosZ;
            bone.parent    = -1;
            bones.push_back(bone);
        }
        
        boneHierarchyElement = skeletonNode->FirstChildElement("bonehierarchy");
        assert(boneHierarchyElement);
        for(boneParentElement = boneHierarchyElement->FirstChildElement("boneparent");boneParentElement;
            boneParentElement = boneParentElement->NextSiblingElement("boneparent"))
        {
            const char* cBoneName;
            const char* cBoneParentName;
            cBoneName = boneParentElement->Attribute("bone");
            cBoneParentName = boneParentElement->Attribute("parent");
            int cBoneIndex = 0 , cBoneParentIndex = 0;
            for (int i=0; i<(int)bones.size(); i++)
            {
                if (bones[i].name == cBoneName) cBoneIndex = i;
                if(bones[i].name == cBoneParentName) cBoneParentIndex=i;
            }
            bones[cBoneIndex].parent = cBoneParentIndex;
        }
        
        for (int i = 0; i < bones.size(); i++)
        {
            int p=bones[i].parent;
            if(p>=0) bones[p].childs.push_back(i);
        }
        
        // build hierarchy out
        animationsElement = skeletonNode->FirstChildElement("animations");assert( animationsElement );
        for( animationElement = animationsElement->FirstChildElement("animation"); animationElement;
            animationElement = animationElement->NextSiblingElement("animation"))
        {
            int result;
            double dAnimationLength=0;
            const char* cAnimationName;
            cAnimationName = animationElement->Attribute("name");
            result = animationElement->QueryDoubleAttribute("length", &dAnimationLength);
            printf("Animation[%ld] Name:[%s] , Length: %3.03f sec \n" ,animations.size(), cAnimationName, (float) dAnimationLength);
            
            Animation animation;
            animation.frameCount=0;
            sprintf(animation.name ,"%s", cAnimationName);
            animation.nameLength = strnlen(animation.name,ANI_NAME_LEN);
            animation.time = dAnimationLength;
            std::vector<Track> &tracks = animation.tracks;
            tracks.resize(bones.size());
            
            TiXmlElement *tracksElement = 0,*trackElement = 0;
            tracksElement = animationElement->FirstChildElement("tracks");
            assert( tracksElement );
            
            for( trackElement = tracksElement->FirstChildElement("track"); trackElement;
                trackElement = trackElement->NextSiblingElement("track"))
            {
                const char* cBoneName;
                cBoneName = trackElement->Attribute("bone");
                size_t trackIndex = 0;
                for (size_t i=0; i<bones.size(); i++) {
                    if(bones[i].name == cBoneName) { trackIndex = i; break;}
                }
                Track &track = tracks[trackIndex];
                
                TiXmlElement* keyframesElement = 0, *keyframeElement = 0;
                keyframesElement = trackElement->FirstChildElement("keyframes");
                assert( keyframesElement );
                
                for( keyframeElement = keyframesElement->FirstChildElement("keyframe"); keyframeElement;
                    keyframeElement = keyframeElement->NextSiblingElement("keyframe"))
                {
                    int result;
                    double dKeyTime;
                    result = keyframeElement->QueryDoubleAttribute("time", &dKeyTime);
                    
                    double dTranslateX = 0, dTranslateY = 0,dTranslateZ = 0;
                    double dAxisAngle = 0,dAxisX = 0,dAxisY = 0,dAxisZ = 0;
                    TiXmlElement *translateElement = 0,*rotateElement = 0,*axisElement = 0;
                    translateElement = keyframeElement->FirstChildElement( "translate" );assert( translateElement );
                    rotateElement = keyframeElement->FirstChildElement( "rotate" );assert( rotateElement );
                    axisElement = rotateElement->FirstChildElement( "axis" );assert( axisElement );
                    result = translateElement->QueryDoubleAttribute( "x", &dTranslateX );
                    result+= translateElement->QueryDoubleAttribute( "y", &dTranslateY );
                    result+= translateElement->QueryDoubleAttribute( "z", &dTranslateZ );
                    result+= rotateElement->QueryDoubleAttribute( "angle", &dAxisAngle );
                    result+= axisElement->QueryDoubleAttribute( "x", &dAxisX );
                    result+= axisElement->QueryDoubleAttribute( "y", &dAxisY );
                    result+= axisElement->QueryDoubleAttribute( "z", &dAxisZ );
                    assert( result == 0 );
                    
                    Key key;
                    key.time = dKeyTime;
                    key.rot  = glm::vec4(dAxisAngle, dAxisX, dAxisY, dAxisZ);
                    key.pos = glm::vec3(dTranslateX, dTranslateY,dTranslateZ);
                    track.keys.push_back(key);
                }
                animation.frameCount = max(animation.frameCount,(unsigned int)track.keys.size());
            }
            animations.push_back(animation);
        }
#endif
    }
    
    
    void WriteTrack(std::ofstream& ofs, vector<Track>& tracks)
    {
        size_t t= tracks.size();
        ofs.write((char*)&t, sizeof(size_t));
        foreach(track, tracks)
        {
            t= track->keys.size();
            ofs.write((char*)&t,sizeof(size_t));
            foreach(key, track->keys)
            {
                ofs.write((char*)&key->time,sizeof(float));
                writevec4(ofs, key->rot);
                writevec3(ofs, key->pos);
            }
        }
    }
    
    void WriteAnimation(vector<Bone> bones, vector<Animation> animations,std::string name)
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            string basedir = "resources/engine/"+name+"/";
            ofs.open(basedir+name+".anim",std::ofstream::binary | std::ios::out);
            unsigned int num = (unsigned int)bones.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            num = (unsigned int)animations.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            for (size_t i=0; i<bones.size(); i++) {
                ofs.write((char*)&bones[i].nameLength,sizeof(char));
                for (size_t i=0; i<ANI_NAME_LEN; i++) {
                    ofs.write((char*)&bones[i].name[i],sizeof(char));
                }
                writearray(ofs, bones[i].rot, 4);
                writearray(ofs, bones[i].pos, 3);
                ofs.write((char*)&bones[i].parent,sizeof(int));
                writemat4(ofs, bones[i].matrix);
                writemat4(ofs, bones[i].invbindmatrix);
                size_t sz=bones[i].childs.size();
                ofs.write((char*)&sz,sizeof(size_t));
                loop0j(sz) ofs.write((char*)&bones[i].childs[j],sizeof(int));
            }
            for (size_t i=0; i<animations.size(); i++) {
                ofs.write((char*)&animations[i].nameLength,sizeof(char));
                for (size_t i=0; i<ANI_NAME_LEN; i++) {
                    ofs.write((char*)&animations[i].name[i],sizeof(char));
                }
                ofs.write((char*)&animations[i].time,sizeof(float));
                WriteTrack(ofs, animations[i].tracks);
                ofs.write((char*)&animations[i].frameCount,sizeof(int));
            }
            ofs.close();
        }
        catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::ANIMATION::SAVE, " <<name<< std::endl;
        }
    }
    
    void caltangent(const Vertex* v1, const Vertex* v2, const Vertex* v3, glm::vec3* tan, glm::vec3* bit)
    {
        caltangent(v1->Position, v2->Position, v3->Position, v1->TexCoords, v2->TexCoords, v3->TexCoords, tan, bit);
    }
    
    void caltangent(const BaseVert3* v1, const BaseVert3* v2, const BaseVert3* v3, glm::vec3* tan, glm::vec3* bit)
    {
        caltangent(v1->Position, v2->Position, v3->Position , v1->TexCoords, v2->TexCoords, v3->TexCoords, tan, bit);
    }
    
    void caltangent(const ColorVertex* v1, const ColorVertex* v2, const ColorVertex* v3, glm::vec3* tan, glm::vec3* bit)
    {
        caltangent(v1->Position, v2->Position, v3->Position , v1->TexCoords, v2->TexCoords, v3->TexCoords, tan, bit);
    }
    
    void caltangent(const CompxVertex* v1, const CompxVertex* v2, const CompxVertex* v3, glm::vec3* tan, glm::vec3* bit)
    {
        caltangent(v1->Position, v2->Position, v3->Position , v1->TexCoords, v2->TexCoords, v3->TexCoords, tan, bit);
    }
    
    void ConvertVertex(Vertex* vert, TangVertex* tanVert)
    {
        tanVert->Position = vert->Position;
        tanVert->Normal = vert->Normal;
        tanVert->TexCoords = vert->TexCoords;
    }
    
    void ConvertVertex(CompxVertex* vert, TangVertex* tanVert)
    {
        tanVert->Position = vert->Position;
        tanVert->Normal = vert->Normal;
        tanVert->TexCoords = vert->TexCoords;
    }
    
}
