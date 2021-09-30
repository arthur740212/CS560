#ifndef QUATERNION_CLASS_H
#define QUATERNION_CLASS_H


#include <glm/glm.hpp>

class Quaternion
{
public:

	Quaternion();
	Quaternion(glm::vec3 image);
	Quaternion(float real, glm::vec3 image);
	~Quaternion();

	Quaternion operator-();
	Quaternion operator*(Quaternion rhs);
	Quaternion operator*(glm::vec3 rhs);

	float s;
	glm::vec3 v;

private:

	

};

void quatTest();


#endif