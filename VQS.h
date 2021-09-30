#ifndef VQS_CLASS_H
#define VQS_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>
#include "quaternion.h"


class VQS
{
public:

	VQS operator*(VQS rhs);
	glm::vec3 operator*(glm::vec3 rhs);


	glm::vec3 position;
	Quaternion quaternion;
	float scale;
private:




};

#endif 