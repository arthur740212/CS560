#pragma once
#ifndef MODELLOADER_CLASS_H
#define MODELLOADER_CLASS_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate|aiProcess_GenSmoothNormals|aiProcess_FlipUVs|aiProcess_JoinIdenticalVertices)


class Model
{
public:
    Model(const char* path);
    ~Model();
   // void Draw(Shader& shader);
    
    
    std::vector<Mesh> meshes;
private:
    // model data
    
    const char* directory;

    void LoadModel(const char* path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene, int layer);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, int layer);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
        const char* typeName);
};


#endif