#ifndef QUATERNION_CLASS_H
#define QUATERNION_CLASS_H


#include <glm/glm.hpp>
#include <iostream>
class Quaternion
{
public:

	Quaternion();
	Quaternion& operator=(const Quaternion& rhs);
	Quaternion(const Quaternion& obj);
	Quaternion(glm::vec3 image);
	Quaternion(float real, glm::vec3 image);
	~Quaternion();

	float Length();
	float SqrLength();
	float Dot(Quaternion rhs);
	void Normalize();
	Quaternion Normalized();
	Quaternion Inverse();

	Quaternion Scale(float c);
	
	Quaternion operator+(Quaternion rhs);
	Quaternion operator*(Quaternion rhs);
	Quaternion operator*(glm::vec3 rhs);

	float s;
	glm::vec3 v;

private:

	

};

void quatTest();

std::ostream& operator<<(std::ostream& out, Quaternion q);

#endif