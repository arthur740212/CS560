  
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;
layout (location = 5) in ivec4 boneIds; 
layout (location = 6) in vec4 weights;

out vec3 curPos;
out vec3 color;
out vec3 normal;
out vec2 texCoord;

uniform mat4 camMatrix;

uniform mat4 model;
uniform mat4 modelOrient;
uniform mat4 modelPos;
uniform mat4 view;
uniform mat4 proj;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    //curPos = vec3(model * vec4(aPos, 1.0f));
	vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
    }

	normal = aNormal;
	color = aColor;
	texCoord = aTex;
	
    mat4 modelMat = modelPos* model* modelOrient;

	//gl_Position = camMatrix *vec4(curPos, 1.0f);
	gl_Position = camMatrix * modelMat * totalPosition;
}