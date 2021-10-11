#include "model.h"

Model::Model(const char* path)
{
    LoadModel(path);
}

void Model::LoadModel(const char* path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, ASSIMP_LOAD_FLAGS);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    else 
    { 
        std::cout << "Import Success"<< std::endl;
    }
    //directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene, 0);
}
Model::~Model()
{
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, int layer)
{
    std::cout << node->mName.C_Str() << std::endl;
   

    //Getvqs(node);
    layer++;
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene, layer));
    }
    // then do the same for each of its children
   
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        for (int j = 0; j < layer; j++)
        {
            std::cout << "-->";
        }
        ProcessNode(node->mChildren[i], scene, layer);
    }

}

//void Model::ProcessNode(aiNode* node, const aiScene* scene)
//{
//    // process all the node's meshes (if any)
//    for (unsigned int i = 0; i < node->mNumMeshes; i++)
//    {
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        meshes.push_back(ProcessMesh(mesh, scene));
//    }
//    // then do the same for each of its children
//    for (unsigned int i = 0; i < node->mNumChildren; i++)
//    {
//        ProcessNode(node->mChildren[i], scene);
//    }
//    std::cout << node->mName.C_Str() << std::endl;
//}
//
//Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
//{
//    std::vector<Vertex> vertices;
//    std::vector<GLuint> indices;
//    std::vector<Texture> textures;
//
//    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//    {
//        Vertex vertex;
//        // process vertex positions, normals and texture coordinates
//        glm::vec3 vector;
//
//        vector.x = mesh->mVertices[i].x;
//        vector.y = mesh->mVertices[i].y;
//        vector.z = mesh->mVertices[i].z;
//        vertex.position = vector;
//
//        vector.x = mesh->mNormals[i].x;
//        vector.y = mesh->mNormals[i].y;
//        vector.z = mesh->mNormals[i].z;
//        vertex.normal = vector;
//
//        SetVertexBoneDataToDefault(vertex);
//
//        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//        {
//            glm::vec2 vec;
//            vec.x = mesh->mTextureCoords[0][i].x;
//            vec.y = mesh->mTextureCoords[0][i].y;
//            vertex.texUV = vec;
//        }
//        else
//        {
//            vertex.texUV = glm::vec2(0.0f, 0.0f);
//        }
//
//        vertices.push_back(vertex);
//    }
//    // process indices
//    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//    {
//        aiFace face = mesh->mFaces[i];
//        for (unsigned int j = 0; j < face.mNumIndices; j++)
//            indices.push_back(face.mIndices[j]);
//    }
//    // process material
//   /* if (mesh->mMaterialIndex >= 0)
//    {
//        [...]
//    }*/
//    ExtractBoneWeightForVertices(vertices, mesh, scene);
//
//    return Mesh(vertices, indices, textures);
//}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, int layer)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        SetVertexBoneDataToDefault(vertex);
        // process vertex positions, normals and texture coordinates
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        vector.x = 0.1f;//* layer;
        vector.y = 1.0f;// - 0.1 * layer;
        vector.z = 1.0f;
        vertex.color = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
        }
        else
        {
            vertex.texUV = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    ExtractBoneWeightForVertices(vertices, mesh, scene);
    return Mesh(vertices, indices, textures);
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.boneIDs[i] = -1;
        vertex.boneWeights[i] = 0.0f;
    }
}
void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
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

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = m_BoneCounter;
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