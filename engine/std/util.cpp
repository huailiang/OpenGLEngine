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
#endif

#define TINYOBJLOADER_IMPLEMENTATION
#include "../ext/tiny_obj_loader.h"

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
            std::cout<<"checkdir:"<<temp<<std::endl;
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
    
    void readv2(ifstream& f, glm::vec2& v)
    {
        f.read((char*)&v.x,sizeof(float));
        f.read((char*)&v.y,sizeof(float));
    }
    
    void readv3(ifstream& f, glm::vec3& v)
    {
        f.read((char*)&(v.x),sizeof(float));
        f.read((char*)&(v.y),sizeof(float));
        f.read((char*)&(v.z),sizeof(float));
    }
    
    void readstring(ifstream& f, string& str)
    {
        size_t len;
        f.read((char*)&len, sizeof(size_t));
        if(len > 0)
        {
            char* temp = new char[len+1];
            f.read(temp, len);
            temp[len] = '\0';
            str = temp;
            delete []temp;
        }
    }
    
    void readtex(ifstream& f, GLuint& id)
    {
        string str;
        readstring(f, str);
        if(!str.empty())
        {
            
#ifdef _GLES_
            str = getPath(str);
#else
            str = "resources/objects/" + curr_obj +"/" + str;
#endif
            TexMgr::getInstance()->LoadTex(str, id);
        }
    }
    
    
    void writestring(ofstream& f, std::string& str)
    {
        size_t len = str.size();
        f.write((char*)&len, sizeof(size_t));
        if(len > 0) f.write(str.c_str(), len);
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
            for(int i = 0; i < mesh.num_face_vertices.size(); i++) {
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
            WriteMesh(shapes[s].name, indices, vertices, texture);
            std::cout<<"export "<<shapes[s].name<<std::endl;
        }
        std::cout<<name<<" export finish"<<std::endl;
        return ret;
    }
    
    
    void WriteSummary(std::vector<tinyobj::shape_t>& shapes)
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            string basedir = "resources/mesh/"+curr_obj+"/";
#ifdef _QT_EDIT_
            basedir = WORKDIR + basedir;
#endif
            CheckDir(basedir.c_str());
            ofs.open(basedir+"summary",std::ofstream::binary | std::ios::out);
            unsigned int num = (unsigned int)shapes.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            for (size_t i =0; i<shapes.size(); i++)
            {
                writestring(ofs, shapes[i].name);
            }
            ofs.close();
        }
        catch (std::ofstream::failure e)
        {
            std::cout << "ERROR::MESH SUMMARY "<< std::endl;
        }
        vector<string> vec;
        ReadSummary(curr_obj, vec);
    }
    
    
    
    void WriteMesh(std::string name,vector<int>& indices, vector<Vertex>& vertices,std::string texture[])
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            string basedir = "resources/mesh/"+curr_obj+"/";
            ofs.open(basedir+name,std::ofstream::binary | std::ios::out);
            unsigned int num = (unsigned int)indices.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            num = (unsigned int)vertices.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            for(int i=0;i<TEXTURE_NUM;i++)  writestring(ofs, texture[i]);
            for(size_t i=0;i<indices.size();i++) {
                 ofs.write((char*)&indices[i],sizeof(unsigned int));
            }
            for (size_t i=0; i<vertices.size(); i++) {
                ofs.write((char*)&vertices[i].Position.x,sizeof(float));
                ofs.write((char*)&vertices[i].Position.y,sizeof(float));
                ofs.write((char*)&vertices[i].Position.z,sizeof(float));
                ofs.write((char*)&vertices[i].TexCoords.x,sizeof(float));
                ofs.write((char*)&vertices[i].TexCoords.y,sizeof(float));
                ofs.write((char*)&vertices[i].Normal.x,sizeof(float));
                ofs.write((char*)&vertices[i].Normal.y,sizeof(float));
                ofs.write((char*)&vertices[i].Normal.z,sizeof(float));
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
            std::string path = "resources/mesh/"+name+"/summary";
#ifdef _QT_EDIT_
            path = WORKDIR + path;
#endif
#ifdef _GLES_
            path = getPath("summary");
#endif
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

    
    MeshData* ReadMesh(std::string name)
    {
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
          std::string path = "resources/mesh/"+curr_obj+"/"+name;
#ifdef _QT_EDIT_
            path = WORKDIR + path;
#endif
#ifdef _GLES_
            path = getPath(name);
#endif
            ifs.open(path, std::ifstream::binary | std::ios::in);
            unsigned int inds = 0,verts = 0;
            ifs.seekg(0, ios::beg);
            ifs.read((char*)(&inds), sizeof(unsigned int));
            ifs.read((char*)(&verts), sizeof(unsigned int));
            
            MeshData* mesh = new MeshData();
            mesh->num_indice = inds;
            mesh->indices = new unsigned int[inds];
            mesh->num_vert = verts;
            mesh->vertices = new Vertex[verts];
            readtex(ifs, mesh->diffuse_texture);
            readtex(ifs, mesh->normal_texure);
            readtex(ifs, mesh->specul_texture);
            readtex(ifs, mesh->ambient_texture);

            for (size_t i=0; i<inds; i++)
            {
                ifs.read((char*)(&mesh->indices[i]), sizeof(unsigned int));
            }
            for (size_t i=0; i<verts; i++) {
                Vertex vertex;
                readv3(ifs, vertex.Position);
                readv2(ifs, vertex.TexCoords);
                readv3(ifs, vertex.Normal);
                mesh->vertices[i] = vertex;
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
}
