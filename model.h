#pragma once
#ifndef MODELLOADER_CLASS_H
#define MODELLOADER_CLASS_H

#include "defConst.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include <map>
#include "VQS.h"
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate|aiProcess_GenSmoothNormals|aiProcess_FlipUVs|aiProcess_JoinIdenticalVertices)

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    int indexInList;

    Vertex boneVertex;
    /*offset matrix transforms vertex from model space to bone space*/
    //glm::mat4 offset;

    VQS offsetVQS;

    bool isIK = false;
};

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
   
    void ProcessNode(aiNode* node, const aiScene* scene, int layer); 
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, int layer);


    //void ProcessNode(aiNode* node, const aiScene* scene);
    //Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    
    /*std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
        const char* typeName);*/

    std::map<std::string, BoneInfo> m_BoneInfoMap; //
    int m_BoneCounter = 0;
public:
    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }

    //Sets the influencing bones list to nothing
    void SetVertexBoneDataToDefault(Vertex& vertex);
    //Adds and influencing bone data to the list
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

    //Get all the vertices influenced by each bone, then add that bone to the vertices affected
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
};


#endif