#ifndef ANIMATOR_CLASS_H
#define ANIMATOR_CLASS_H

#include "animation.h"
#include "VQS.h"
#include <glm/gtx/string_cast.hpp>


class Animator
{
public:
    Animator(Animation* Animation);

    void UpdateAnimation(float dt);

    void PlayAnimation(Animation* pAnimation);

    void CalculateBoneVQS(const AssimpNodeData* node, VQS parentVQS);

   /* void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
    {
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = (node->transformation);
        Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(m_CurrentTime);
            nodeTransform = Bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < node->childrenCount; i++)
            CalculateBoneTransform(&node->children[i], globalTransformation);
    }*/

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        for (int i = 0; i < 100; i++)
        {
            m_FinalBoneMatrices[i] = m_FinalBoneVQSes[i].VQStoMatrix();
        }
        return m_FinalBoneMatrices;
    }

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::vector<VQS> m_FinalBoneVQSes;
    std::vector <Vertex> SkeletonVertices;
    std::vector <GLuint> SkeletonIndices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
};

#endif