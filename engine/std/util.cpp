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
#include "../engineio.h"

namespace engine
{
    string curr_obj;
    map<string,GLuint> loaded_textures;
    
    void WriteSummary(std::vector<tinyobj::shape_t>& shapes);
    void WriteSkeleton(vector<Bone>& bones, vector<Animation>& animations,const std::string name);
    
    int CheckDir(const char* path)
    {
#if defined(__APPLE__) && !defined(_GLES_)
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
        return 0;
    }
    
    void getTextures(tinyobj::mesh_t& mesh, std::vector<tinyobj::material_t>& materials,std::string* vec)
    {
        int id = mesh.material_ids[0];
        if(id>=0)
        {
            tinyobj::material_t mat = materials[id];
            vec[0] = mat.diffuse_texname;
            vec[1] = mat.normal_texname;
            vec[2] = mat.specular_texname;
            vec[3] = mat.ambient_texname;
        }
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
            Texture(str.c_str(), (EXT)ext, &id);
        }
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
            uint num = (uint)shapes.size();
            ofs.write((char*)&num,sizeof(uint));
            MODEL_TYPE type = MODEL_OBJ;
            ofs.write((char*)&type, sizeof(MODEL_TYPE));
            for (size_t i =0; i<shapes.size(); i++)
            {
                string name = shapes[i].name;
                if (name.empty()) name = curr_obj;
                writestring(ofs, name);
            }
            ofs.close();
        }catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MESH SUMMARY "<< std::endl;
        }
    }
    
    void WriteMaterial(const string name,const std::string texture[],std::string dir)
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            if(dir.empty()) dir = curr_obj;
            string basedir = "resources/engine/"+dir+"/";
            ofs.open(basedir+name+".mat",std::ofstream::binary | std::ios::out);
            for(int i=0;i<TEXTURE_NUM;i++)
            {
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
        } catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MATERIAL::SAVE, " <<name<< std::endl;
        }
    }
    
    void writeVert3(std::ofstream& ofs, const VertType type, const VertType t, float* ptr)
    {
        if(type & t)
        {
            ofs.write((char*)ptr++,sizeof(float));
            ofs.write((char*)ptr++,sizeof(float));
            ofs.write((char*)ptr++,sizeof(float));
        }
    }
    
    void WriteVertex(std::ofstream& ofs, Vertex* vert,VertType type)
    {
       if(type & Vt_Pos3) writevec3(ofs, vert->Position);
       if(type & Vt_UV) writevec2(ofs, vert->TexCoords);
       if(type & Vt_Normal) writevec3(ofs, vert->Normal);
    }
    
    void WriteSkinVertex(std::ofstream& ofs, SkinVertex* vert)
    {
        Vertex* vertex = (Vertex*)vert;
        WriteVertex(ofs, vertex, Vt_Pos3 | Vt_UV | Vt_Normal);
        error_stop(vert->bonecount<=3, "skin vert inner error");
        glm::vec3 weight(0), boneidx(0);
        for (int i=0; i<vert->bonecount; i++) {
            weight[i] = vert->weight[i];
            boneidx[i] = vert->boneindx[i];
        }
        glm::vec4 bone = glm::vec4(weight, 65025 * boneidx.x + 255 * boneidx.y + boneidx.z);
        writevec4(ofs, bone);
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
            short lod = 1;
            ofs.write((char*)&lod,sizeof(short));
            uint num = (uint)vertices.size();
            ofs.write((char*)&num,sizeof(uint));
            ofs.write((char*)&type,sizeof(VertType));
            num = (uint)indices.size();
            ofs.write((char*)&num,sizeof(uint));
            loop0i(indices.size()) ofs.write((char*)&indices[i],sizeof(uint));
            loop0i(vertices.size()) WriteVertex(ofs, &vertices[i], type);
            ofs.close();
        } catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MESH::SAVE, " <<name<< std::endl;
        }
    }
    
    void WriteSkinMesh(const std::string name,vector<vector<int>>& indices, vector<SkinVertex> vertices, std::string dir)
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            if(dir.empty()) dir = curr_obj;
            string basedir = std::string(WORKDIR) + "resources/engine/"+dir+"/";
            CheckDir(basedir.c_str());
            
            // summary
            ofs.open(basedir+"summary.sum",std::ofstream::binary | std::ios::out);
            uint num = 1;
            ofs.write((char*)&num,sizeof(uint));
            num= MODEL_XML | MODEL_ANI;
            ofs.write((char*)&num, sizeof(MODEL_TYPE));
            writestring(ofs, name);
            ofs.close();
            
            //materail
            std::string texture[4];
            texture[0]="masterchief_base.jpg";
            texture[1] = texture[2] = texture[3] = "";
            WriteMaterial(name, texture, name);
            
            // mesh
            ofs.open(basedir+name+".mesh",std::ofstream::binary | std::ios::out);
            short lod = (short)indices.size();
            ofs.write((char*)&lod, sizeof(short));
            num=(uint)vertices.size();
            ofs.write((char*)&num,sizeof(uint));
            num=0x2111;
            ofs.write((char*)&num,sizeof(VertType));
            loop0i(indices.size())
            {
                num = (uint)indices[i].size();
                ofs.write((char*)&num,sizeof(uint));
                loop0j(num) ofs.write((char*)&(indices[i][j]),sizeof(int));
            }
            loop0i(vertices.size()) WriteSkinVertex(ofs, &vertices[i]);
            ofs.close();
            std::cout<<"save skin mesh "<<name<<std::endl;
        } catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MESH::SAVE, " <<name<< std::endl;
        }
    }
    
    MODEL_TYPE ReadSummary(const std::string name, vector<string>& items)
    {
        MODEL_TYPE type= MODEL_OBJ;
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            curr_obj = name;
            std::string path = getResPath("engine/"+name+"/summary.sum");
            ifs.open(path, std::ifstream::binary | std::ios::in);
            uint num = 0;
            items.clear();
            ifs.seekg(0, ios::beg);
            ifs.read((char*)(&num), sizeof(uint));
            ifs.read((char*)&type, sizeof(MODEL_TYPE));
            string str;
            for (size_t i=0; i<num; i++) {
                readstring(ifs, str);
                items.push_back(str);
            }
        } catch (std::ifstream::failure e)
        {
            std::cerr<<"read summary error "<<name<<std::endl;
        }
        return type;
    }
    
    bool LoadObj(const std::string name)
    {
        curr_obj = name;
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;
        std::string base_dir =  std::string(WORKDIR)+"resources/objects/"+name+"/";
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (base_dir + name +".obj").c_str(), base_dir.c_str());
        if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
        if (!err.empty()) { std::cerr << err << std::endl; return false; }
        WriteSummary(shapes);
        for (size_t s=0; s<shapes.size(); s++)
        {
            tinyobj::mesh_t mesh = shapes[s].mesh;
            vector<int> indices;
            vector<int> old_indices;
            vector<Vertex> vertices;
            size_t index_offset = 0;
            int indx_new = 0;
            bool has_texcoord = attrib.texcoords.size() > 0;
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
                        vertex.TexCoords.x =has_texcoord ? attrib.texcoords[idx.texcoord_index * 2 + 0] : 0;
                        vertex.TexCoords.y = has_texcoord ?attrib.texcoords[idx.texcoord_index * 2 + 1] : 0;
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
            string name = shapes[s].name;
            if(name.empty()) name = curr_obj;
            WriteMesh(name, indices, vertices, has_texcoord?(Vt_Pos3|Vt_UV|Vt_Normal):(Vt_Pos3|Vt_Normal));
            WriteMaterial(name,texture);
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
            if(mat->mesh==nullptr)
            {
                mat->mesh = ReadMesh(name);
            }
            ifs.close();
        } catch (std::ifstream::failure e)
        {
            std::cerr<<"read material error "<<name<<std::endl;
        }
    }
    
    void RecalcuteLod(MeshData* data, const std::string name, const std::string objdir,const short ilod)
    {
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            if(!objdir.empty()) curr_obj = objdir;
            std::string path = getResPath("engine/"+curr_obj+"/"+name+".mesh");
            ifs.open(path, std::ifstream::binary | std::ios::in);
            uint inds = 0,verts = 0, type = 0x0;
            ifs.seekg(0, ios::beg);
            short lod = 1;
            ifs.read((char*)&lod, sizeof(lod));
            ifs.read((char*)(&verts), sizeof(uint));
            ifs.read((char*)(&type), sizeof(uint));
            error_stop(ilod<lod, "read lod error");
            short curr=0;
            while (curr < ilod) {
                ifs.read((char*)&inds,sizeof(uint));
                ifs.seekg(inds*4, std::ios_base::cur);
                curr++;
            }
            ifs.read((char*)(&inds), sizeof(uint));
            delete [] data->indices;
            data->num_indice = inds;
            data->indices = new unsigned short[inds];
            loop0i(inds) ifs.read((char*)(&(data->indices[i])), sizeof(uint));
            ifs.close();
        } catch (std::ifstream::failure e)
        {
            std::cerr<<"read mesh error "<<name<<std::endl;
        }
    }
    
    MeshData* ReadMesh(const std::string name, const std::string objdir, const short ilod)
    {
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            if(!objdir.empty()) curr_obj = objdir;
            std::string path = getResPath("engine/"+curr_obj+"/"+name+".mesh");
            ifs.open(path, std::ifstream::binary | std::ios::in);
            uint inds = 0,verts = 0, type = 0x0;
            ifs.seekg(0, ios::beg);
            short lod = 1;
            ifs.read((char*)&lod, sizeof(lod));
            ifs.read((char*)(&verts), sizeof(uint));
            ifs.read((char*)(&type), sizeof(uint));
            error_stop(ilod<lod, "read lod error");
            short curr=0;
            while (curr < ilod) {
                ifs.read((char*)&inds,sizeof(uint));
                ifs.seekg(inds, std::ios_base::cur);
                curr++;
            }
            ifs.read((char*)(&inds), sizeof(uint));
            MeshData* mesh = new MeshData();
            mesh->type = type;
            mesh->num_indice = inds;
            mesh->indices = new unsigned short[inds];
            mesh->num_vert = verts;
            mesh->vertices = nullptr;
            loop0i(inds) ifs.read((char*)(&(mesh->indices[i])), sizeof(uint));
            for (short i=ilod+1; i<lod; i++) {
                uint num =0;
                ifs.read((char*)&num,sizeof(uint));
                ifs.seekg(num*4, std::ios_base::cur);
            }
            
            switch (type) {
                case 0x0111:
                    mesh->vertices = new Vertex[verts];
                    ifs.read((char*)(mesh->vertices), sizeof(Vertex) * verts);
                    break;
                case 0x0101:
                    mesh->vertices = new NormalVert[verts];
                    ifs.read((char*)(mesh->vertices), sizeof(NormalVert) * verts);
                    break;
                case 0x0012:
                    mesh->vertices = new BaseVert2[verts];
                    ifs.read((char*)(mesh->vertices), sizeof(BaseVert2) * verts);
                    break;
                case 0x0011:
                    mesh->vertices = new BaseVert3[verts];
                    ifs.read((char*)(mesh->vertices), sizeof(BaseVert3) * verts);
                    break;
                case 0x1011:
                    mesh->vertices = new ColorVertex[verts];
                    ifs.read((char*)(mesh->vertices), sizeof(ColorVertex) * verts);
                    break;
                case 0x1111:
                    mesh->vertices = new CompxVertex[verts];
                    ifs.read((char*)(mesh->vertices), sizeof(CompxVertex) * verts);
                    break;
                case 0x2111:
                    mesh->vertices = new SkeletonVertex[verts];
                    ifs.read((char*)(mesh->vertices), sizeof(SkeletonVertex) * verts);
                    break;
                default:
                    std::cerr<<"vertex config not support format: 0x"<<hex<<type<<std::endl;
                    break;
            }
            ifs.close();
            return mesh;
        } catch (std::ifstream::failure e)
        {
            std::cerr<<"read mesh error "<<name<<std::endl;
        }
        return nullptr;
    }
    
    void LoadXmlObj(const char* file)
    {
#ifndef _GLES_
        std::string fname = std::string(file);
        std::string path = std::string(WORKDIR)+"resources/xml/"+fname+".mesh.xml";
        FILE* fn= std::fopen(path.c_str(), "rb");
        if(!fn) std::cerr<<"xml mesh not found: " <<path <<std::endl;
        char line[1000],name[1000];
        memset(line, 0, 100);
        memset(name, 0, 100);
        
        vector<vector<int>> indices;
        std::vector<SkinVertex> vertices;
        int x,y,z;
        float f1,f2,f3;
        uint normal_id = 0, texcoord_id = 0;
        int lod = 0;
        vector<int> v; indices.push_back(v);
        while(fgets(line, 1000, fn) != NULL)
        {
            if(sscanf(line," <face v1=\"%d\" v2=\"%d\" v3=\"%d\" />",&x,&y,&z) == 3)
            {
                indices[lod].push_back(x); indices[lod].push_back(y); indices[lod].push_back(z);
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
                int i=vertices[x].bonecount;
                if(i < 3)
                {
                    vertices[x].weight[i] = f1;
                    vertices[x].boneindx[i] = y;
                    vertices[x].bonecount++;
                }
            }
            if(sscanf(line," <lodgenerated value=\"%f\">",&f1)==1)
            {
                vector<int> v; indices.push_back(v);
                lod++;
            }
        }
        
        fclose(fn);
        std::cout<<"load xml "<<fname<<", statistic face:"<<indices.size()/3<<" verts:"<<vertices.size()<<std::endl;
        WriteSkinMesh(fname, indices, vertices, fname);
        LoadXmlSkeleton(file);
#endif
    }
    
    void LoadXmlSkeleton(const char* file)
    {
#ifndef _GLES_
        std::vector<Animation> animations;
        std::vector<Bone> bones;
        std::string path = std::string(WORKDIR) + "resources/xml/"+std::string(file)+".skeleton.xml";
        
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
        error_stop(node, "skeleton is nil");
        skeletonNode = node->ToElement();
        error_stop(skeletonNode, "skeletonNode ptr 0");
        
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
            positionElement = boneElement->FirstChildElement("position");assert(positionElement);
            rotateElement = boneElement->FirstChildElement("rotation");assert(rotateElement);
            axisElement = rotateElement->FirstChildElement("axis");assert( axisElement);
            result+= positionElement->QueryDoubleAttribute("x", &dPosX);
            result+= positionElement->QueryDoubleAttribute("y", &dPosY);
            result+= positionElement->QueryDoubleAttribute("z", &dPosZ);
            result+= rotateElement->QueryDoubleAttribute("angle", &dAxisAngle);
            result+= axisElement->QueryDoubleAttribute("x", &dAxisX);
            result+= axisElement->QueryDoubleAttribute("y", &dAxisY);
            result+= axisElement->QueryDoubleAttribute("z", &dAxisZ);
            assert(result == 0);
            
            Bone bone;
            bone.name = cBoneName;
            bone.rot[0] = dAxisAngle;
            bone.rot[1] = dAxisX;
            bone.rot[2] = dAxisY;
            bone.rot[3] = dAxisZ;
            bone.pos[0] = dPosX;
            bone.pos[1] = dPosY;
            bone.pos[2] = dPosZ;
            bone.parent = -1;
            bones.push_back(bone);
        }
        
        boneHierarchyElement = skeletonNode->FirstChildElement("bonehierarchy");
        error_stop(boneHierarchyElement, "not fond bonehierarchy in xml");
        for(boneParentElement = boneHierarchyElement->FirstChildElement("boneparent");boneParentElement;
            boneParentElement = boneParentElement->NextSiblingElement("boneparent"))
        {
            const char* cBoneName;
            const char* cBoneParentName;
            cBoneName = boneParentElement->Attribute("bone");
            cBoneParentName = boneParentElement->Attribute("parent");
            int cBoneIndex = 0, cBoneParentIndex = 0;
            for (int i=0; i<(int)bones.size(); i++)
            {
                if (bones[i].name == cBoneName) cBoneIndex = i;
                if(bones[i].name == cBoneParentName) cBoneParentIndex = i;
            }
            bones[cBoneIndex].parent = cBoneParentIndex;
        }
        
        for (int i = 0; i < bones.size(); i++)
        {
            int p=bones[i].parent;
            if(p>=0) bones[p].childs.push_back(i);
        }
        
        // build hierarchy out
        animationsElement = skeletonNode->FirstChildElement("animations");assert(animationsElement);
        for( animationElement = animationsElement->FirstChildElement("animation"); animationElement;
            animationElement = animationElement->NextSiblingElement("animation"))
        {
            int result;
            double dAnimationLength=0;
            const char* cAnimationName = animationElement->Attribute("name");
            result = animationElement->QueryDoubleAttribute("length", &dAnimationLength);
            printf("Animation[%ld] Name:[%s] , Length: %3.03f sec \n" ,animations.size(), cAnimationName, (float) dAnimationLength);
            
            Animation animation;
            animation.frameCount = 0;
            animation.name = std::string(cAnimationName);
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
                loop0i(bones.size()) if(bones[i].name == cBoneName) { trackIndex = i; break;}
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
                animation.frameCount = max(animation.frameCount,(uint)track.keys.size());
            }
            animations.push_back(animation);
        }
        WriteSkeleton(bones, animations, file);
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
    
    void ReadTrack(std::ifstream& ifs, XAnimation* anim)
    {
        ifs.read((char*)&anim->num_track,sizeof(size_t));
        anim->tracks = new XTrack[anim->num_track];
        for (uint i=0; i<anim->num_track; i++) {
            ifs.read((char*)&anim->tracks[i].num_key,sizeof(size_t));
            anim->tracks[i].keys=new Key[anim->tracks[i].num_key];
            for (uint j=0; j<anim->tracks[i].num_key; j++) {
                ifs.read((char*)&anim->tracks[i].keys[j].time,sizeof(float));
                readvec4(ifs, anim->tracks[i].keys[j].rot);
                readvec3(ifs, anim->tracks[i].keys[j].pos);
            }
        }
    }
    
    void WriteSkeleton(vector<Bone>& bones, vector<Animation>& animations, const std::string name)
    {
#ifndef _GLES_
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            string basedir = std::string(WORKDIR) + "resources/engine/"+name+"/";
            ofs.open(basedir+name+".anim",std::ofstream::binary | std::ios::out);
            uint num = (uint)bones.size();
            ofs.write((char*)&num,sizeof(uint));
            num = (uint)animations.size();
            ofs.write((char*)&num,sizeof(uint));
            std::cout<<" write bones:"<<bones.size()<<" anim:"<<animations.size()<<std::endl;
            for (size_t i=0; i<bones.size(); i++) {
                writestring(ofs, bones[i].name);
                writearray(ofs, bones[i].rot, 4);
                writearray(ofs, bones[i].pos, 3);
                ofs.write((char*)&bones[i].parent,sizeof(int));
                writemat4(ofs, bones[i].matrix);
                writemat4(ofs, bones[i].invbindmatrix);
                size_t sz= bones[i].childs.size();
                ofs.write((char*)&sz,sizeof(int));
                loop0j(sz) ofs.write((char*)&(bones[i].childs[j]),sizeof(int));
            }
            for (size_t i=0; i<animations.size(); i++) {
                writestring(ofs, animations[i].name);
                ofs.write((char*)&animations[i].time,sizeof(float));
                WriteTrack(ofs, animations[i].tracks);
                ofs.write((char*)&animations[i].frameCount,sizeof(uint));
            }
            ofs.close();
        } catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::ANIMATION::SAVE, " <<name<< std::endl;
        }
#endif
    }
    
    void ReadSkeleton(Skeleton* skeleton, const std::string name, const std::string dir)
    {
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            if(!dir.empty()) curr_obj = dir;
            std::string path = getResPath("engine/"+curr_obj+"/"+name+".anim");
            ifs.open(path, std::ifstream::binary | std::ios::in);
            ifs.seekg(0, ios::beg);
            ifs.read((char*)(&skeleton->num_bone), sizeof(uint));
            ifs.read((char*)(&skeleton->num_anim), sizeof(uint));
            skeleton->bones = new XBone[skeleton->num_bone];
            skeleton->animations = new XAnimation[skeleton->num_anim];
            for (uint i=0; i<skeleton->num_bone; i++) {
                readstring(ifs, skeleton->bones[i].name);
                readarray(ifs, skeleton->bones[i].rot, 4);
                readarray(ifs, skeleton->bones[i].pos, 3);
                ifs.read((char*)&skeleton->bones[i].parent,sizeof(int));
                readmat4(ifs, skeleton->bones[i].matrix);
                readmat4(ifs, skeleton->bones[i].invbindmatrix);
                ifs.read((char*)&skeleton->bones[i].num_child,sizeof(int));
                if(skeleton->bones[i].num_child) skeleton->bones[i].childs = new int[skeleton->bones[i].num_child];
                loop0j(skeleton->bones[i].num_child) ifs.read((char*)(skeleton->bones[i].childs+j),sizeof(int));
            }
            for (uint i=0; i<skeleton->num_anim; i++) {
                readstring(ifs, skeleton->animations[i].name);
                ifs.read((char*)&skeleton->animations[i].time, sizeof(float));
                ReadTrack(ifs, skeleton->animations+i);
                ifs.read((char*)&skeleton->animations[i].frameCount,sizeof(uint));
            }
            ifs.close();
        }  catch (std::ifstream::failure e)
        {
            std::cerr<<"read skeleton error "<<name<<std::endl;
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
