#include "VQS.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, VQS vqs)
{

	std::cout << "pos : (" << vqs.position.x << "," << vqs.position.y << "," << vqs.position.z << ")" << std::endl;
	std::cout << "quat : (" << vqs.quaternion.s << "(" << vqs.quaternion.v.x << "," << vqs.quaternion.v.y << "," << vqs.quaternion.v.z << ")" << std::endl;
	std::cout << "scale : " << vqs.scale << std::endl;
	return out;
}

std::ostream& operator<<(std::ostream& out, glm::vec3 vqs)
{
	std::cout << "vec3 : (" << vqs.x << "," << vqs.y << "," << vqs.z << ")" << std::endl;
	return out;
}
VQS::VQS() 
{
	position = glm::vec3(0);
	quaternion = Quaternion(0.0f, glm::vec3(1.0)).Normalized();
	scale = 1.0f;
}
VQS::VQS(glm::vec3 _position,Quaternion _quaternion,float _scale) 
{
	position = _position;
	quaternion = _quaternion.Normalized();
	scale = _scale;
}

VQS VQS::operator*(VQS rhs)
{
	VQS result;

	result.scale = scale * rhs.scale;
	result.quaternion = quaternion * rhs.quaternion;
	result.position = *this * (rhs.position);

	return result;
}

glm::vec3 VQS::operator*(glm::vec3 rhs)
{
	return (quaternion * (rhs * scale) * quaternion.Inverse()).v + position;
}

VQS VQS::Inverse()
{
	VQS temp(glm::vec3(0), quaternion.Inverse(), 1.0f / scale);
	glm::vec3 invPos = -(temp * position);
	return VQS(invPos, quaternion.Inverse(), 1.0f / scale);

}


void VQStest() 
{
	VQS vqs1;
	VQS vqs2(glm::vec3(1.0f, 8.7f, 9.5f), Quaternion(3.0f, glm::vec3(2.7f, 9.9f, 5.4f)), 2.1f);

	std::cout << vqs1 << std::endl;
	std::cout << vqs2 << std::endl;
	std::cout << vqs2.Inverse() << std::endl;
	std::cout << vqs2.Inverse() * vqs2 << std::endl;
	std::cout << vqs1 * vqs2.Inverse().position << std::endl;

}