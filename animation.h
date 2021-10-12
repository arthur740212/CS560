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

    //Add the bones to a map following the hierarchical node structure
    void ReadBones(const aiAnimation* animation, Model& model);

    //Create the node hierarchy
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
    


    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};




#endif
