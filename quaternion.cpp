#include "quaternion.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, Quaternion q)
{
	std::cout << q.s << "(" << q.v.x << "," << q.v.y << "," << q.v.z << ")" << std::endl;
	return out;
}

Quaternion::Quaternion() 
{
	s = 1.0f;
	v = glm::vec3(0);
}
Quaternion::Quaternion(glm::vec3 image) 
{
	s = 0.0f;
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


float Quaternion::Length()
{
	return sqrt(SqrLength());
}

float Quaternion::SqrLength()
{
	return s * s + glm::dot(v, v);
}

void Quaternion::Normalize()
{
	float invLength = 1.0f / Length();

	s *= invLength;
	v *= invLength;
}

Quaternion Quaternion::Normalized()
{
	float invLength = 1.0f / Length();

	Quaternion normalized(s, v);
	normalized.s *= invLength;
	normalized.v *= invLength;

	return normalized;
}

Quaternion Quaternion::Scale(float c) 
{
	return Quaternion(s * c, v * c);
}

Quaternion Quaternion::Inverse()
{
	Quaternion conjugate(this->s, -this->v);
	return conjugate.Scale(1.0f / SqrLength());
}

Quaternion Quaternion::operator*(Quaternion rhs)
{
	Quaternion result;

	result.s = s * rhs.s - glm::dot(v, rhs.v);
	result.v = s * rhs.v + v * rhs.s + glm::cross(v, rhs.v);

	return result;
}

Quaternion Quaternion::operator*(glm::vec3 rhs)
{
	return operator*(Quaternion(rhs));
}


void quatTest() 
{
	Quaternion q1(0.7f, glm::vec3(0, 0.8f, 2.9f));
	Quaternion q2(glm::vec3(0, 0.2f, 2.1f));
	Quaternion q3 = q1.Normalized();
	q2.Normalize();
	Quaternion q4 = q1.Inverse();
	Quaternion q5 = q2 * q2.Inverse();
	Quaternion q6 = q3 * q3.Inverse();
	Quaternion q7 = q2 * q3.v;

	std::cout << q1 << std::endl;
	std::cout << q2 << std::endl;
	std::cout << q3 << std::endl;
	std::cout << q4 << std::endl;
	std::cout << q5 << std::endl;
	std::cout << q6 << std::endl;
	std::cout << q7 << std::endl;
	std::cout << q1 << std::endl;
	std::cout << q2 << std::endl;
}