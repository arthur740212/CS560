#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 2) in vec3 aColor;
layout (location = 5) in ivec4 boneIds; 

uniform mat4 camMatrix;

uniform mat4 model;

const int MAX_BONES = 100;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec3 curPos;
out vec3 color;

void main()
{
	curPos = vec3(model * vec4(aPos, 1.0f));
	color=aColor;

	vec4 localPosition = finalBonesMatrices[boneIds[0]] * vec4(aPos,1.0f);

	gl_Position = camMatrix * model * localPosition;
	//gl_Position = camMatrix * vec4(curPos, 1.0f);
}