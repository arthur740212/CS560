#ifndef VQS_CLASS_H
#define VQS_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>
#include <vector>


class VQS
{
public:

private:
	
	glm::vec3 position;
	glm::quat quaternion;
	glm::float64 scale;

	
};

glm::vec3 operator*(glm::vec3 lhs, const VQS& rhs)
{

}

#endif 