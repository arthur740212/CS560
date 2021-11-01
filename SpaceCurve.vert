#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 2) in vec3 aColor;

uniform mat4 camMatrix;



out vec3 curPos;
out vec3 color;

void main()
{
	color=aColor;

	vec4 localPosition = vec4(aPos,1.0f);

	gl_Position = camMatrix * localPosition;
}