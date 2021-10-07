#ifndef BONE_CLASS_H
#define BONE_CLASS_H

#include "quaternion.h"
#include <vector>
#include <string>
#include "Libraries/include/assimp/anim.h"

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    Quaternion orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
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
    std::string m_Name;
    int m_ID;

    Bone(const std::string& name, int ID, const aiNodeAnim* channel);

};


#endif