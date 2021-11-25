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

    bool isIK = false;
};

struct Manipulator
{
    std::vector<std::string> nodeNames;
    std::vector<int> priority;
    std::string priorityString;
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

    inline const Manipulator& GetManip() { return m_manip; }

    //Gets the vertices of the bones for point display
    void GetSkeletonBones();

    //Gets the links between bone vertices for line display
    void GetSkeletonBoneHiearchy(const AssimpNodeData* node, int parentBoneIndex);
  
    std::vector<Vertex> m_SkeletonBones;
    std::vector<GLuint> m_SkeletonBonesIndices;
    std::vector<GLuint> m_SkeletonBoneLineIndices;
    std::vector<Texture> m_SkeletonBonesTextures;

    //void SetIKRoot(AssimpNodeData& dest, std::string name);
    //void SetIKLinks(AssimpNodeData& dest);

    //Find the node/bone by name, and set it that it is calculated by IK
    void SetIsIK(AssimpNodeData& dest, std::string name);

    //Find the parent of the node/bone, returns the name of the parent
    std::string FindParentNodeName(AssimpNodeData& dest, std::string name);

    //Creates a list of joints controlled by IK, starting from the end effector
    void SetManipulator(AssimpNodeData& dest, std::string name, int count);

    //Sets the order of priority, index 0 ~ size - 1 (high ~ low priority)  stores the index of the joints
    //default is 0 ~ size-1
    void SetDefaultPriority();

    //this randomizes the priorities
    void SetNewPriority();

    //helper to represent the priority orden into a string 
    std::string SetPriorityString();

private:

    //Add the bones to a map following the hierarchical node structure
    void ReadBones(const aiAnimation* animation, Model& model);

    //Create the node hierarchy
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
    


    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    Manipulator m_manip;
};




#endif
