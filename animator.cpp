#include "animator.h"

Animator::Animator(Animation* Animation)
{
    m_CurrentTime = 0.0;
    m_CurrentAnimation = Animation;

    //m_FinalBoneMatrices.reserve(100);
    m_FinalBoneVQSes.reserve(100);

    for (int i = 0; i < 100; i++)
    {
        //m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
        m_FinalBoneVQSes.push_back(VQS());
    }
}

void Animator::CalculateBoneVQS(const AssimpNodeData* node, VQS parentVQS)
{
    std::string nodeName = node->name;
    VQS nodeVQS = (node->vqsTrans);
    Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(m_CurrentTime);
        nodeVQS = Bone->GetLocalVQS();
    }

    VQS globalVQS = parentVQS * nodeVQS;

    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        VQS offsetVQS = boneInfoMap[nodeName].offsetVQS;
        m_FinalBoneVQSes[index] = globalVQS * offsetVQS;
        boneInfoMap[nodeName].boneVertex.position = m_FinalBoneVQSes[index].position;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneVQS(&node->children[i], globalVQS);
}

void Animator::UpdateAnimation(float dt)
{
    m_DeltaTime = dt;
    if (m_CurrentAnimation)
    {
        m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
        CalculateBoneVQS(&m_CurrentAnimation->GetRootNode(), VQS());
    }
}

void Animator::PlayAnimation(Animation* pAnimation)
{
    m_CurrentAnimation = pAnimation;
    m_CurrentTime = 0.0f;
}