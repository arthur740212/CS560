#include "animation.h"
#include "time.h"

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

    //Sets the end effector "Bip01_L_Finger12", and 7 nodes/bones above that as joints 
    SetManipulator(m_RootNode, "Bip01_L_Finger12",7);
    //Sets the default priority (lower for each parent) 
    SetDefaultPriority();

    for (auto iter = m_BoneInfoMap.begin(); iter != m_BoneInfoMap.end(); iter++)
    {
        std::cout << iter->first << " " << iter->second.isIK << std::endl;
    }
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

//void Animation::SetIKRoot(AssimpNodeData& dest,std::string name)
//{
//    for (int i = 0; i < dest.childrenCount; i++)
//    {
//        if (dest.name == name) 
//        { 
//            dest.isIK = true; 
//            Bone* findingBone = FindBone(dest.name);
//            if (findingBone)
//            {
//                m_BoneInfoMap[dest.name].isIK = dest.isIK;
//            }
//            return; 
//        }
//        SetIKRoot(dest.children[i], name);
//    }
//}

//void Animation::SetIKLinks(AssimpNodeData& dest)
//{
//    for (int i = 0; i < dest.childrenCount; i++)
//    {
//        if (dest.isIK)
//        {
//            dest.children[i].isIK = true;
//        }
//
//        Bone* findingBone = FindBone(dest.children[i].name);
//        if (findingBone)
//        {
//            m_BoneInfoMap[dest.children[i].name].isIK = dest.isIK;
//        }
//
//        //std::cout << dest.name << " " << dest.isIK << std::endl;
//        SetIKLinks(dest.children[i]);
//    }
//  
//}

void Animation::SetIsIK(AssimpNodeData& dest, std::string name)
{
    //Finds the bone that matches the name, returns when found
    if (dest.name == name)
    {
        dest.isIK = true;

        Bone* findingBone = FindBone(name);
        if (findingBone)
        {
            m_BoneInfoMap[name].isIK = dest.isIK;
        }
        return;
    }

    //Keep on searching through children if not found
    for (int i = 0; i < dest.childrenCount; i++)
    {

        SetIsIK(dest.children[i], name);
    }
}

std::string Animation::FindParentNodeName(AssimpNodeData& dest, std::string name)
{
    for (int i = 0; i < dest.childrenCount; i++)
    {
        //if there is a children matches the name, return this node as the parent
        if (dest.children[i].name == name)
        {
            return dest.name;
        }
        //Keep on searching through children if not found
        std::string parentName = FindParentNodeName(dest.children[i], name);

        //returns once it is found
        if (parentName != name) { return parentName; }
    }

    //returns the name in search if we can't find one matching in childrens 
    return name;
}

void Animation::SetManipulator(AssimpNodeData& dest, std::string name, int count)
{

    //Pushes the end effector into the list of OK nodes
    SetIsIK(m_RootNode, name);
    m_manip.nodeNames.push_back(name);

    
    std::string parentName = name;

    //for every added node, find its parent and add it to the list, till there is no parent
    for (int i = 0; i < count; i++)
    {
        parentName = FindParentNodeName(m_RootNode, parentName);
        SetIsIK(m_RootNode, parentName);

        if (m_manip.nodeNames.back() != parentName) { m_manip.nodeNames.push_back(parentName); }
        else { break; }
    }

    //Allocate a list of priority
    for (int k = 0; k < m_manip.nodeNames.size() - 1; k++)
    {
        m_manip.priority.push_back(k);
    }

    //Set the priority to default
    SetDefaultPriority();
}

void Animation::SetDefaultPriority()
{
    for (int k = 0; k < m_manip.nodeNames.size() - 1; k++)
    {
        m_manip.priority[k] = k;
    }
}

int Random(int size)
{
    return rand() % size;
}

void Animation::SetNewPriority()
{
    time_t t;
    srand(int(time(&t)));

    int size = m_manip.priority.size();
    for (int k = 0; k < size; k++)
    {
        m_manip.priority[k] = size;
    }

    for (int k = 0; k < size; k++)
    {
        int random = Random(size);

        while (m_manip.priority[random] < k)
        {
            random=(random+1)%size;
        }

        m_manip.priority[random] = k;
    }
}

std::string Animation::SetPriorityString()
{
    std::string PriorString;
    int size = m_manip.priority.size();
    for (int k = 0; k < size; k++)
    {
        PriorString += std::to_string(m_manip.priority[k]);
    }
    return PriorString;
}