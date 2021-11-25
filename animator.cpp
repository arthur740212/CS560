#include "animator.h"



VQS CCD(glm::vec3 targetPos, VQS localVQS, VQS parentVQS)
{
    //Convert the target position to the parents coordinate
    glm::vec3 targetAtParentSpaceparent = parentVQS.Inverse() * targetPos;

    //the end effector position, which is already at the parents coordinate
    glm::vec3 localPos = localVQS.position;

    //Do the rotation with a clamping factor, to make the movement smooth instead of directly snapping
    float cos = glm::dot(localPos, targetAtParentSpaceparent) / glm::length(localPos) / glm::length(targetAtParentSpaceparent);
    cos = glm::clamp(cos, -1.0f, 1.0f);
    float arccos = acos(cos);
    std::cout << arccos << std::endl;
    arccos = glm::clamp(arccos, -0.01f, 0.01f);
    glm::vec3 rotAxis = glm::normalize(glm::cross(localPos, targetAtParentSpaceparent));
   
    //converting a glm::mat4 into my VQS classs
    VQS decomp;
    decomp.decomposeMtx(glm::mat4_cast(glm::angleAxis(arccos, rotAxis)));

    return decomp * localVQS;
}



Animator::Animator(Animation* Animation)
{
    m_CurrentTime = 0.0;
    m_CurrentAnimation = Animation;
    m_current_manip = Animation->GetManip();

    //m_FinalBoneMatrices.reserve(100);
    m_LocalBoneVQSes.reserve(100);
    m_FinalBoneVQSes.reserve(100);
    m_GlobalBoneVQSes.reserve(100);

    for (int i = 0; i < 100; i++)
    {
        //m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
        m_LocalBoneVQSes.push_back(VQS());
        m_FinalBoneVQSes.push_back(VQS());
        m_GlobalBoneVQSes.push_back(VQS());
    }
    CalculateBoneVQS(&m_CurrentAnimation->GetRootNode(), VQS());
}

void Animator::CalculateBoneVQS(const AssimpNodeData* node, VQS parentVQS)
{
    std::string nodeName = node->name;
    VQS nodeVQS = (node->vqsTrans);
    Bone* Bone = m_CurrentAnimation->FindBone(nodeName);
    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();

    if (Bone)
    {
        Bone->Update(m_CurrentTime);
        nodeVQS = Bone->GetLocalVQS();
        m_LocalBoneVQSes[boneInfoMap[nodeName].id] = nodeVQS;
    }

    VQS globalVQS = parentVQS * nodeVQS;

    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        VQS offsetVQS = boneInfoMap[nodeName].offsetVQS;
        m_FinalBoneVQSes[index] = globalVQS * offsetVQS;
        m_GlobalBoneVQSes[index] = globalVQS;

        //Can be commented out
        boneInfoMap[nodeName].boneVertex.position = m_FinalBoneVQSes[index].position;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneVQS(&node->children[i], globalVQS);
}


void Animator::CalculateBoneVQSAndIK(const AssimpNodeData* node, VQS parentVQS)
{
    std::string nodeName = node->name;
    VQS nodeVQS = (node->vqsTrans);
    Bone* Bone = m_CurrentAnimation->FindBone(nodeName);
    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();

    if (Bone)
    {
        if (!boneInfoMap[nodeName].isIK) 
        {
            Bone->Update(m_CurrentTime);
        }
        nodeVQS = Bone->GetLocalVQS();
    }

    VQS globalVQS = parentVQS * nodeVQS;

    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        VQS offsetVQS = boneInfoMap[nodeName].offsetVQS;
        m_FinalBoneVQSes[index] = globalVQS * offsetVQS;
        m_GlobalBoneVQSes[index] = globalVQS;

        //Can be commented out
        boneInfoMap[nodeName].boneVertex.position = m_FinalBoneVQSes[index].position;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneVQSAndIK(&node->children[i], globalVQS);
}

void Animator::DoIK(glm::vec3 target)
{
    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();

    //run through the priority list, start with the node with hightest priority
    for (int priorityIndex = 0; priorityIndex <
        m_current_manip.priority.size()
        ; priorityIndex++)
    {
        //this is the joint number up from the end effector
        int jointIndex = m_current_manip.priority[priorityIndex];

        //Finds the GLobalVQS for the parent of the joint
        int index2 = boneInfoMap[m_current_manip.nodeNames[jointIndex + 1]].id;
        VQS LinkGlobalVQS = m_GlobalBoneVQSes[index2];

        VQS LinkLocalVQS = VQS();

        //Starting from the joint, we multiply each of its childrens localVQS, to get the VQS from the joint to the end effector
        for (int n = jointIndex; n > -1; n--)
        {
            LinkLocalVQS = LinkLocalVQS * m_CurrentAnimation->FindBone(m_current_manip.nodeNames[n])->m_LocalVQS;
        }


        //We do CCD to the VQS/vector(joint - end effector) from above
        VQS wholeLinkLocalVQS = CCD(target, LinkLocalVQS, LinkGlobalVQS);

        //Multiply with localVQS Inverses in opposite order, to retreive the localVQS of the joint after CCD rotation
        for (int j = 0; j < jointIndex; j++)
        {
            wholeLinkLocalVQS = wholeLinkLocalVQS * (m_CurrentAnimation->FindBone(m_current_manip.nodeNames[j])->m_LocalVQS.Inverse());
        }


        //Update the localVQS of the joint
        Bone* Bone = m_CurrentAnimation->FindBone(m_current_manip.nodeNames[jointIndex]);
        Bone->m_LocalVQS = wholeLinkLocalVQS;
        m_LocalBoneVQSes[boneInfoMap[m_current_manip.nodeNames[jointIndex]].id] = wholeLinkLocalVQS;

        //Update the joints that are rotated by the link before next calculation
        for (int k = jointIndex; k > -1; k--)
        {
            int boneID = boneInfoMap[m_current_manip.nodeNames[k]].id;
            int parentBoneID = boneInfoMap[m_current_manip.nodeNames[k + 1]].id;

            m_GlobalBoneVQSes[boneID] = m_GlobalBoneVQSes[parentBoneID] * m_CurrentAnimation->FindBone(m_current_manip.nodeNames[k])->m_LocalVQS;
            m_FinalBoneVQSes[boneID] = m_GlobalBoneVQSes[boneID] * boneInfoMap[m_current_manip.nodeNames[k]].offsetVQS;
        }
    }

    //Then calculates all the VQSes for each bone for shader usage
    CalculateBoneVQSAndIK(&m_CurrentAnimation->GetRootNode(), VQS());
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