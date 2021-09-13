#version 330 core

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

in vec3 color;
in vec2 texCoord;
in vec3 curPos;
in vec3 normal;

out vec4 FragColor;

void main()
{

	float ambient = 0.2f;
	float specLight=0.5f;

	vec3 normal = normalize(normal);
	vec3 lightDir= normalize(lightPos - curPos);
	float diffuse = max(dot(normal, lightDir), 0.0f);

	vec3 viewDir=normalize(camPos-curPos);
	vec3 reflectDir=reflect(-lightDir,normal);
	float specAmount = pow(max(dot(viewDir, reflectDir), 0.0f),8);
	float specular=specAmount*specLight;

	FragColor = (texture(tex0, texCoord) * (diffuse + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
}