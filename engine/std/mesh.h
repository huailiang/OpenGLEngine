#ifndef MESH_H
#define MESH_H

#include "../common.h"
#include "shader.h"
#include "material.h"

using namespace std;

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
    
public:

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    
    void Draw(const Shader* shader);

    void DrawMesh();

private:
    
    void SetupMesh();

public:
    
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

private:
    
    unsigned int VBO, EBO;
    
};


#endif
