#ifndef MESH_H
#define MESH_H

#include "../common.h"
#include "shader.h"

using namespace std;

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

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
