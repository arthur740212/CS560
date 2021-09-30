#include "quaternion.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, Quaternion q)
{
	std::cout << q.s << "(" << q.v.x << "," << q.v.y << "," << q.v.z << ")" << std::endl;
	return out;
}

Quaternion::Quaternion() 
{
	s = 0;
	v = glm::vec3(0);
}
Quaternion::Quaternion(glm::vec3 image) 
{
	s = 0;
	v = image;
}
Quaternion::Quaternion(float real, glm::vec3 image) 
{
	s = real;
	v = image;
}
Quaternion::~Quaternion() 
{
}

Quaternion Quaternion::operator-()
{
	Quaternion result(this->s, -this->v);
	return result;
}

Quaternion Quaternion::operator*(Quaternion rhs)
{
	Quaternion result;

	result.s = s * rhs.s - glm::dot(v, rhs.v);
	result.v = s * rhs.v + v * rhs.s + glm::cross(v, rhs.v);

	std::cout << rhs << std::endl;

	return result;
}

Quaternion Quaternion::operator*(glm::vec3 rhs)
{
	return operator*(Quaternion(rhs));
}


void quatTest() 
{
	Quaternion q1;
	Quaternion q2(glm::vec3(0, 0.2f, 2.1f));
	Quaternion q3(0.7f, glm::vec3(0, 0.8f, 2.9f));
	Quaternion q4 = -q3;
	Quaternion q5 = q2 * -q2;
	Quaternion q6 = q3 * q4;
	Quaternion q7 = q2 * q3.v;

	std::cout << q1 << std::endl;
	std::cout << q2 << std::endl;
	std::cout << q3 << std::endl;
	std::cout << q4 << std::endl;
	std::cout << q5 << std::endl;
	std::cout << q6 << std::endl;
	std::cout << q7 << std::endl;
}