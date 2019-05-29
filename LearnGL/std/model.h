#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "light.h"


using namespace std;
using namespace glm;

class Model
{
    
public:
    
    vector<Texture> textures_loaded;    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    Model(string const &path, bool gamma = false);
    
    mat4 GetModelMatrix(vec3 pos, vec3 scale, float angle);

    void Draw(Shader* shader, Camera* camera, Light* light, vec3 pos, vec3 scale, float angle);
    
    void DrawShadow(Shader* shader, Camera* camera, Light* light, vec3 pos, vec3 scale, float angle);

private:
    
    void renderModel(Shader* shader, Camera* camera, Light* light, vec3 pos, vec3 scale, float angle);
    
    void loadModel(string const &path);

    void processNode(aiNode *node, const aiScene *scene);
    
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

private:
    
    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    
    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
};


#endif
