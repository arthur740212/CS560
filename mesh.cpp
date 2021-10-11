#include "mesh.h"
#include "defConst.h"
Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures) 
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	
	VBO VBO(vertices);
	EBO EBO(indices);

	VAO.LinkVBO(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkVBO(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkVBO(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkVBO(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));
	VAO.LinkVBO(VBO, 5, MAX_BONE_INFLUENCE, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
	VAO.LinkVBO(VBO, 6, MAX_BONE_INFLUENCE, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
	
}

void Mesh::DrawPoint(Shader& shader, Camera& camera)
{
	shader.Activate();
	VAO.Bind();


	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.Matrix(shader, "camMatrix");

	// Draw the actual mesh
	//glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	glPointSize(7.5f);
	glDrawArrays(GL_POINTS, indices[0], indices.size());
}

void Mesh::DrawLine(Shader& shader, Camera& camera)
{
	shader.Activate();
	VAO.Bind();


	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.Matrix(shader, "camMatrix");

	// Draw the actual mesh
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Draw(Shader& shader, Camera& camera) 
{
	shader.Activate();
	VAO.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].TextureUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.Matrix(shader, "camMatrix");

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}