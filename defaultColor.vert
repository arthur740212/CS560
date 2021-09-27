#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 2) in vec3 aColor;


uniform mat4 camMatrix;

uniform mat4 model;

out vec3 curPos;
out vec3 color;

void main()
{
	curPos = vec3(model * vec4(aPos, 1.0f));
	color=aColor;
	gl_Position = camMatrix * vec4(curPos, 1.0f);
}