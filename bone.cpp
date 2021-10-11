#include "bone.h"
#include <iostream>

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
    :
    m_Name(name),
    m_ID(ID),
    m_LocalTransform(1.0f)
{
    m_NumPositions = channel->mNumPositionKeys;

    for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
        data.timeStamp = timeStamp;
        m_Positions.push_back(data);
    }

    m_NumRotations = channel->mNumRotationKeys;
    for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = glm::quat(aiOrientation.w, glm::vec3(aiOrientation.x, aiOrientation.y, aiOrientation.z));
        data.qua = Quaternion(aiOrientation.w, glm::vec3(aiOrientation.x, aiOrientation.y, aiOrientation.z));
      /*  std::cout << glm::to_string(data.orientation) << std::endl;
        std::cout << data.qua << std::endl;*/
        data.timeStamp = timeStamp;
        m_Rotations.push_back(data);
    }

    m_NumScalings = channel->mNumScalingKeys;
    for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        float timeStamp = channel->mScalingKeys[keyIndex].mTime;
        KeyScale data;
        data.scale = glm::vec3(scale.x, scale.y, scale.z);
        data.uniScale = scale.x;
        data.timeStamp = timeStamp;
        m_Scales.push_back(data);
    }
}

void Bone::Update(float animationTime)
{

    //glm::mat4 translation = InterpolatePosition(animationTime);
    //glm::mat4 rotation = InterpolateRotation(animationTime);
    //glm::mat4 scale = InterpolateScaling(animationTime);
    //m_LocalTransform = translation * rotation * scale;

    VQS local(GetInterpolatedPos(animationTime), GetInterpolatedQuat(animationTime), GetInterpolatedScale(animationTime));
    m_LocalTransform = local.VQStoMatrix();
}

glm::vec3 Bone::GetInterpolatedPos(float animationTime)
{
    if (1 == m_NumPositions)
    {
        return  m_Positions[0].position;
    }
    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float t = (animationTime - m_Positions[p0Index].timeStamp) / (m_Positions[p1Index].timeStamp - m_Positions[p0Index].timeStamp);
    glm::vec3 finalPosition = (1 - t) * m_Positions[p0Index].position + t * m_Positions[p1Index].position;
    return finalPosition;
}
Quaternion Bone::GetInterpolatedQuat(float animationTime)
{
    if (1 == m_NumRotations)
    {
        return m_Rotations[0].qua;
    }
    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    Quaternion q1(m_Rotations[p0Index].qua.Normalized());
    Quaternion q2(m_Rotations[p1Index].qua.Normalized());
    float t = (animationTime - m_Positions[p0Index].timeStamp) / (m_Positions[p1Index].timeStamp - m_Positions[p0Index].timeStamp);
    float dotProd = q1.Dot(q2);
    if (dotProd > 1.0f) { dotProd = 1.0f; }
    if (dotProd < -1.0f) { dotProd = -1.0f; }
    float alpha = acos(dotProd);
    float sinAlpha = sin(alpha);
    Quaternion finQua;
    if (sinAlpha != 0)
    {
        finQua = q1.Scale(sin((1 - t) * alpha) / sinAlpha) + q2.Scale(sin(t * alpha) / sinAlpha);
    }
    else
    {
        finQua = q1;
    }
    finQua.Normalize();
    return finQua;
}
float Bone::GetInterpolatedScale(float animationTime)
{
    if (1 == m_NumScalings)
    {
        return m_Scales[0].uniScale;
    }
    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float t = (animationTime - m_Positions[p0Index].timeStamp) / (m_Positions[p1Index].timeStamp - m_Positions[p0Index].timeStamp);
    float finalScale = (1 - t) * m_Scales[p0Index].uniScale + t * m_Scales[p1Index].uniScale;
    return finalScale;
}

int Bone::GetPositionIndex(float animationTime)
{
    for (int index = 0; index < m_NumPositions - 1; ++index)
    {
        if (animationTime < m_Positions[index + 1].timeStamp)
            return index;
    }
    assert(0);
}
int Bone::GetRotationIndex(float animationTime)
{
    for (int index = 0; index < m_NumRotations - 1; ++index)
    {
        if (animationTime < m_Rotations[index + 1].timeStamp)
            return index;
    }
    assert(0);
}
int Bone::GetScaleIndex(float animationTime)
{
    for (int index = 0; index < m_NumScalings - 1; ++index)
    {
        if (animationTime < m_Scales[index + 1].timeStamp)
            return index;
    }
    assert(0);
}