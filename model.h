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


glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    Vertex boneVertex;
    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

    VQS offsetVQS;
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
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene, int layer);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, int layer);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
        const char* typeName);

    std::map<std::string, BoneInfo> m_BoneInfoMap; //
    int m_BoneCounter = 0;
public:
    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }

    void SetVertexBoneDataToDefault(Vertex& vertex)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            vertex.boneIDs[i] = -1;
            vertex.boneWeights[i] = 0.0f;
        }
    }
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            if (vertex.boneIDs[i] < 0)
            {
                vertex.boneWeights[i] = weight;
                vertex.boneIDs[i] = boneID;
                break;
            }
        }
    }

    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
    {
        for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = m_BoneCounter;
                //newBoneInfo.vqs.Decompose(mesh->mBones[boneIndex]->mOffsetMatrix);
                newBoneInfo.offset = ConvertMatrixToGLMFormat(
                    mesh->mBones[boneIndex]->mOffsetMatrix);
                newBoneInfo.offsetVQS.Decompose(mesh->mBones[boneIndex]->mOffsetMatrix);
                newBoneInfo.boneVertex.position = newBoneInfo.offsetVQS.Inverse().position;
                newBoneInfo.boneVertex.color = glm::vec3(0.0f, 0.7f, 0.0f);
                newBoneInfo.boneVertex.boneIDs[0] = m_BoneCounter;
                m_BoneInfoMap[boneName] = newBoneInfo;
                boneID = m_BoneCounter;
                m_BoneCounter++;
            }
            else
            {
                boneID = m_BoneInfoMap[boneName].id;
            }
            assert(boneID != -1);
            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= vertices.size());
                SetVertexBoneData(vertices[vertexId], boneID, weight);
            }
        }
    }

};


#endif