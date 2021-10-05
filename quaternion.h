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

	float Length();
	float SqrLength();
	void Normalize();
	Quaternion Normalized();
	Quaternion Inverse();

	Quaternion Scale(float c);
	
	Quaternion operator*(Quaternion rhs);
	Quaternion operator*(glm::vec3 rhs);

	float s;
	glm::vec3 v;

private:

	

};

void quatTest();


#endif