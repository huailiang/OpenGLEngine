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
    
    void writeV3(std::ofstream& ofs, const VertType type, const VertType t, float* ptr)
    {
        if((type & t) > 0)
        {
            ofs.write((char*)ptr++,sizeof(float));
            ofs.write((char*)ptr++,sizeof(float));
            ofs.write((char*)ptr++,sizeof(float));
        }
    }
    
    void WriteMesh(const std::string name,vector<int>& indices, vector<Vertex>& vertices)
    {
        std::ofstream ofs;
        ofs.exceptions (std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            string basedir = "resources/engine/"+curr_obj+"/";
            ofs.open(basedir+name+".mesh",std::ofstream::binary | std::ios::out);
            unsigned int num = (unsigned int)indices.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            num = (unsigned int)vertices.size();
            ofs.write((char*)&num,sizeof(unsigned int));
            num = 0x0111; //mesh type
            ofs.write((char*)&num,sizeof(unsigned int));
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
            WriteMesh(shapes[s].name, indices, vertices);
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
    
}
