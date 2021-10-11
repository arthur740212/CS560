#ifndef BONE_CLASS_H
#define BONE_CLASS_H

#include "quaternion.h"
#include <vector>
#include <string>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "Libraries/include/assimp/anim.h"
#include "Libraries/include/glm/gtx/transform.hpp"
#include "Libraries/include/glm/gtx/quaternion.hpp"
#include "Libraries/include/glm/gtx/norm.hpp"
#include "VQS.h"

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    Quaternion qua;
    //glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    float uniScale;
    //glm::vec3 scale;
    float timeStamp;
};

class Bone
{
private:
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;
    int m_NumPositions;
    int m_NumRotations;
    int m_NumScalings;

    glm::mat4 m_LocalTransform;
    VQS m_LocalVQS;
    std::string m_Name;
    int m_ID;

public:
    Bone(const std::string& name, int ID, const aiNodeAnim* channel);

    void Update(float animationTime);

    glm::vec3 GetInterpolatedPos(float animationTime);
    Quaternion GetInterpolatedQuat(float animationTime);
    float GetInterpolatedScale(float animationTime);

    glm::mat4 GetLocalTransform() { return m_LocalTransform; }
    VQS GetLocalVQS() { return m_LocalVQS; }
    std::string GetBoneName() const { return m_Name; }
    int GetBoneID() { return m_ID; }


    /* Gets the current index on mKeyPositions to interpolate to based on
    the current animation time*/
    int GetPositionIndex(float animationTime);

    /* Gets the current index on mKeyRotations to interpolate to based on the
    current animation time*/
    int GetRotationIndex(float animationTime);

    /* Gets the current index on mKeyScalings to interpolate to based on the
    current animation time */
    int GetScaleIndex(float animationTime);

private:
     
    /*float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    glm::mat4 InterpolatePosition(float animationTime)
    {
        if (1 == m_NumPositions)
            return glm::translate(glm::mat4(1.0f), m_Positions[0].position);
        int p0Index = GetPositionIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
            m_Positions[p1Index].timeStamp, animationTime);
        glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position,
            m_Positions[p1Index].position, scaleFactor);
        return glm::translate(glm::mat4(1.0f), finalPosition);
    }

    glm::mat4 InterpolateRotation(float animationTime)
    {
        if (1 == m_NumRotations)
        {
            auto rotation = glm::normalize(m_Rotations[0].orientation);
            return glm::toMat4(rotation);
        }

        int p0Index = GetRotationIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
            m_Rotations[p1Index].timeStamp, animationTime);
        glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation,
            m_Rotations[p1Index].orientation, scaleFactor);
        finalRotation = glm::normalize(finalRotation);
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
        std::cout << glm::to_string(finalRotation) << std::endl;
        std::cout << q1.Dot(q2)<<" "<< alpha << "\n" << finQua << std::endl;
        return glm::toMat4(finalRotation);
    }

    glm::mat4 InterpolateScaling(float animationTime)
    {
        if (1 == m_NumScalings)
            return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

        int p0Index = GetScaleIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
            m_Scales[p1Index].timeStamp, animationTime);
        glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale
            , scaleFactor);
        return glm::scale(glm::mat4(1.0f), finalScale);
    }
    */
};


#endif