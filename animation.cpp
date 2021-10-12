#include "animation.h"

Animation::Animation(const std::string& animationPath, Model* model)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    auto animation = scene->mAnimations[0];
    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;
    ReadHeirarchyData(m_RootNode, scene->mRootNode);
    ReadBones(animation, *model);
}

Bone* Animation::FindBone(const std::string& name)
{
    auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
        [&](const Bone& Bone)
        {
            return Bone.GetBoneName() == name;
        }
    );
    if (iter == m_Bones.end()) return nullptr;
    else return &(*iter);
}

void Animation::GetSkeletonBones()
{
    int i = 0;
    for (auto iter = m_BoneInfoMap.begin(); iter != m_BoneInfoMap.end(); iter++)
    {
        iter->second.indexInList = i;
        m_SkeletonBones.push_back(iter->second.boneVertex);
        m_SkeletonBonesIndices.push_back(i);
        i++;
    }
}

void Animation::GetSkeletonBoneHiearchy(const AssimpNodeData* node, int parentBoneIndex)
{
    std::string nodeName = node->name;

    auto boneInfoMap = GetBoneIDMap();
    int index = parentBoneIndex;
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        index = boneInfoMap[nodeName].indexInList;
    }

    if (index != 0 && parentBoneIndex != 0)
    {
        m_SkeletonBoneLineIndices.push_back(index);
        m_SkeletonBoneLineIndices.push_back(parentBoneIndex);
    }
    for (int i = 0; i < node->childrenCount; i++)
        GetSkeletonBoneHiearchy(&node->children[i], index);
}

void Animation::ReadBones(const aiAnimation* animation, Model& model)
{
    int size = animation->mNumChannels;

    auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_Bones.push_back(Bone(channel->mNodeName.data,
            boneInfoMap[channel->mNodeName.data].id, channel));
    }

    m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
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