#include "VQS.h"

VQS VQS::operator*(VQS rhs)
{
	VQS result;

	result.scale = scale * rhs.scale;
	result.quaternion = quaternion * rhs.quaternion;
	result.position = operator*(rhs.position);

	return result;
}

glm::vec3 VQS::operator*(glm::vec3 rhs)
{
	glm::vec3 a = rhs * scale;
	return (quaternion * a * -quaternion).v;
}