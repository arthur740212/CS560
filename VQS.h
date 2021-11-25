#ifndef VQS_CLASS_H
#define VQS_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>
#include "quaternion.h"
#include "Libraries/include/assimp/scene.h"




class VQS
{
public:

	VQS();
	VQS(glm::vec3 _position,
		Quaternion _quaternion,
		float _scale);

	glm::mat4 VQStoMatrix();

	VQS operator*(VQS rhs);
	glm::vec3 operator*(glm::vec3 rhs);

	VQS Inverse();

	void decomposeMtx(const glm::mat4& m);
	void Decompose(aiMatrix4x4 aiMat);

	glm::vec3 position;
	Quaternion quaternion;
	float scale;
private:




};

void VQStest();
std::ostream& operator<<(std::ostream& out, VQS vqs);
std::ostream& operator<<(std::ostream& out, glm::vec3 vqs);
#endif 