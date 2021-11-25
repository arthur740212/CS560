#include "VQS.h"
#include <iostream>
#include "Libraries/include/glm/gtx/transform.hpp"
#include "Libraries/include/glm/gtx/quaternion.hpp"
#include <glm/gtx/string_cast.hpp>

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
	quaternion = Quaternion(1.0f, glm::vec3(0.0f)).Normalized();
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
	result.quaternion = (quaternion * rhs.quaternion).Normalized();
	result.position = *this * (rhs.position);
	return result;
}

glm::vec3 VQS::operator*(glm::vec3 rhs)
{
	return (quaternion * (rhs * scale) * quaternion.Inverse()).v + position;
}

glm::mat4 VQS::VQStoMatrix() 
{
	glm::mat4 matPos, matRot, matScl;

	matScl = glm::scale(glm::mat4(1.0f) , glm::vec3(scale, scale, scale));
	matPos = glm::translate(glm::mat4(1.0f), position);
	glm::quat rotation = glm::normalize(glm::quat(quaternion.s, quaternion.v.x, quaternion.v.y, quaternion.v.z));
	matRot = glm::toMat4(rotation);
	
	glm::mat4 output = matPos * matRot * matScl;
	//std::cout<< glm::to_string(output) << std::endl;
	return output;
}

VQS VQS::Inverse()
{
	VQS temp(glm::vec3(0), quaternion.Inverse(), 1.0f / scale);
	glm::vec3 invPos = -(temp * position);
	return VQS(invPos, quaternion.Inverse(), 1.0f / scale);

}

void VQS::decomposeMtx(const glm::mat4& m)
{
	position = m[3];
	glm::vec3 tscale;
	for (int i = 0; i < 3; i++)
	{
		tscale[i] = glm::length(glm::vec3(m[i]));
	}
	scale = tscale[0];
	const glm::mat3 rotMtx(
		glm::vec3(m[0]) / tscale[0],
		glm::vec3(m[1]) / tscale[1],
		glm::vec3(m[2]) / tscale[2]);
	glm::quat rot = glm::quat_cast(rotMtx);
	quaternion.s = rot.w;
	quaternion.v = glm::vec3(rot.x, rot.y, rot.z);
}

void VQS::Decompose(aiMatrix4x4 aiMat)
{
	aiVector3D v;
	aiVector3D s;
	aiQuaternion q;

	aiMat.Decompose(s, q, v);

	position.x = v.x;
	position.y = v.y;
	position.z = v.z;
	quaternion.s = q.w;
	quaternion.v = glm::vec3(q.x, q.y, q.z);
	
	scale = s.x;

	//std::cout << *this << std::endl;
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