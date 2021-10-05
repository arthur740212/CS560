#ifndef VQS_CLASS_H
#define VQS_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>
#include "quaternion.h"




class VQS
{
public:

	VQS();
	VQS(glm::vec3 _position,
		Quaternion _quaternion,
		float _scale);

	VQS operator*(VQS rhs);
	glm::vec3 operator*(glm::vec3 rhs);

	VQS Inverse();


	glm::vec3 position;
	Quaternion quaternion;
	float scale;
private:




};

void VQStest();

#endif 