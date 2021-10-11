#ifndef ANIMATION_CLASS_H
#define ANIMATION_CLASS_H

#include "Libraries/include/glm/glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "model.h"
#include "bone.h"
#include "VQS.h"



struct AssimpNodeData
{
    //glm::mat4 transformation;
    VQS vqsTrans;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:
    Animation() = default;

    Animation(const std::string& animationPath, Model* model);

    ~Animation()
    {
    }

    Bone* FindBone(const std::string& name);


    inline float GetTicksPerSecond() { return m_TicksPerSecond; }

    inline float GetDuration() { return m_Duration; }

    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

    inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
    {
        return m_BoneInfoMap;
    }

    //Gets the vertices of the bones for point display
    void GetSkeletonBones();

    //Gets the links between bone vertices for line display
    void GetSkeletonBoneHiearchy(const AssimpNodeData* node, int parentBoneIndex);
  
    std::vector<Vertex> m_SkeletonBones;
    std::vector<GLuint> m_SkeletonBonesIndices;
    std::vector<GLuint> m_SkeletonBoneLineIndices;
    std::vector<Texture> m_SkeletonBonesTextures;

private:
    void ReadMissingBones(const aiAnimation* animation, Model& model);

    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
    {
        assert(src);

        dest.name = src->mName.data;
        //dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
        dest.vqsTrans.Decompose(src->mTransformation);

        dest.childrenCount = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            ReadHeirarchyData(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }


    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};




#endif
