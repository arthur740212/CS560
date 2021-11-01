#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "mesh.h"
#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "shaderClass.h"
#include "spaceCurve.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"
#include "camera.h"
#include "VQS.h"
#include "quaternion.h"
#include "animation.h"
#include "animator.h"
#include "speedtime.h"

#include <glm/gtx/string_cast.hpp>

// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};



unsigned int width = 1600;
unsigned int height = 900;



int main()
{

	float deltaTime = 0;
	float lastFrame = glfwGetTime();

	//glm::mat4 mat1;
	//glm::mat4 mat2;
	//glm::mat4 mat3 = mat1 * mat2;

	//std::cout << glm::to_string(mat1) << std::endl;
	//std::cout << glm::to_string(mat2) << std::endl;
	//std::cout << glm::to_string(mat3) << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Proj1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);

	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Model model1("stamp.x");
	Animation anim1("stamp.x", &model1);
	Animator animor1(&anim1);
	anim1.GetSkeletonBones();
	anim1.GetSkeletonBoneHiearchy(&anim1.GetRootNode(), 0);


	SpaceCurve spaceCurve;
	std::vector<Texture> dummyTextures;
	
	Mesh spaceCurveLine(spaceCurve.spaceCurveVertices, spaceCurve.spaceCurveIndices, dummyTextures);
	Mesh spaceCurveControlPoints(spaceCurve.controlPointVertices, spaceCurve.controlPointIndices, dummyTextures);

	Mesh skeleton(anim1.m_SkeletonBones, anim1.m_SkeletonBonesIndices, anim1.m_SkeletonBonesTextures);
	Mesh skeletonLine(anim1.m_SkeletonBones, anim1.m_SkeletonBoneLineIndices, anim1.m_SkeletonBonesTextures);

	Texture modeltexture[]
	{
		Texture("brick.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
	};
	std::vector <Texture> modtex(modeltexture, modeltexture + sizeof(modeltexture) / sizeof(Texture));
	model1.meshes[0].textures = modtex;
	
	Shader shaderProgram("default.vert", "default.frag");

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);

	Shader lightShader("light.vert", "light.frag");

	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Crate light mesh
	Mesh light(lightVerts, lightInd, tex);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	glm::mat4 modelSize = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.02f, 0.02f));
	glm::mat4 modelOrient = glm::mat4(1.0f);
	glm::mat4 modelPos = glm::mat4(1.0f);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Shader ColorShader("defaultColor.vert", "defaultColor.frag");
	ColorShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));

	Shader CurveShader("SpaceCurve.vert", "SpaceCurve.frag");
	CurveShader.Activate();

	bool drawTriangle = true;
	bool drawFrame = false;
	bool drawBone = true;
	bool animPause = false;
	float rotation = 0.0f;

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 3.0f, 15.0f));

	float onCurveUnit = 0.0f;
	float arcRatio = 0.0f;
	float facingUnit = 0.0f;

	float curveTime = 0.0f;
	SpeedTime speedtime(10.0f, 24.0f, 30.0f);

	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float FPS = 1 / deltaTime;

		curveTime += deltaTime;
		if (curveTime >= speedtime.tmax) { curveTime -= speedtime.tmax; }
		arcRatio = speedtime.getArcRatioFromDeltaTime(curveTime);

		if (!animPause)
		{
			animor1.UpdateAnimation((speedtime.getSpeedFromDeltaTime(curveTime) + 0.03f) / 7.0f);
		}
		glClearColor(0.7f, 0.7f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!io.WantCaptureMouse)
		{
			//application input handling
			camera.Inputs(window);
		}
		shaderProgram.Activate();

		camera.UpdateMatrix(45.0f, 0.01f, 1000.0f);
		camera.Matrix(shaderProgram, "camMatrix");

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

		ColorShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelSize));
		glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, "modelPos"), 1, GL_FALSE, glm::value_ptr(modelPos));
		glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, "modelOrient"), 1, GL_FALSE, glm::value_ptr(modelOrient));
		//floor.Draw(ColorShader, camera);

		if (drawTriangle)
		{
			onCurveUnit = spaceCurve.EvalArc(arcRatio);
			facingUnit = onCurveUnit + 0.1f;
			if (facingUnit > CONTROL_POINT_COUNT) { facingUnit -= CONTROL_POINT_COUNT; }
			glm::vec3 curPos = spaceCurve.Interpolate(onCurveUnit);
			modelPos = glm::translate(glm::mat4(1.0f), curPos);
			modelOrient = glm::inverse(glm::lookAt(spaceCurve.Interpolate(facingUnit), curPos, glm::vec3(0.0f, 1.0f, 0.0f)));

			printf("%f %f %f\n", curveTime, arcRatio, onCurveUnit);
			//modelOrient = glm::lookAt(curPos, glm::vec3(0), glm::vec3(0.0f, -1.0f, 0.0f));
			shaderProgram.Activate();
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelSize));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "modelPos"), 1, GL_FALSE, glm::value_ptr(modelPos));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "modelOrient"), 1, GL_FALSE, glm::value_ptr(modelOrient));
			auto transforms = animor1.GetFinalBoneVQSes();
			for (int i = 0; i < transforms.size(); ++i)
			{
				std::string finalBoneMatrixString = "finalBonesMatrices[" + std::to_string(i) + "]";
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, finalBoneMatrixString.c_str()), 1, GL_FALSE, glm::value_ptr(transforms[i].VQStoMatrix()));
			}

			ColorShader.Activate();
			for (int i = 0; i < transforms.size(); ++i)
			{
				std::string finalBoneMatrixString = "finalBonesMatrices[" + std::to_string(i) + "]";
				glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, finalBoneMatrixString.c_str()), 1, GL_FALSE, glm::value_ptr(transforms[i].VQStoMatrix()));
			}

			for (int i = 0; i < model1.meshes.size(); i++)
			{

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (drawFrame)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				model1.meshes[i].Draw(shaderProgram, camera);
				glClear(GL_DEPTH_BUFFER_BIT);
				if (drawBone)
				{
					skeleton.DrawPoint(ColorShader, camera);
					skeletonLine.DrawLine(ColorShader, camera);
				}

			}

		}
		ColorShader.Activate();
		//spaceCurve.UpdateSpaceCurve();
		spaceCurveLine.RebindVertices(spaceCurve.spaceCurveVertices);
		spaceCurveControlPoints.RebindVertices(spaceCurve.controlPointVertices);
		spaceCurveLine.DrawLine(CurveShader, camera);
		spaceCurveControlPoints.DrawPoint(CurveShader, camera);

		light.Draw(lightShader, camera);

		ImGui::Begin("Hello Imgui");
		ImGui::Text("WASD to move, SHIFT to speed up");
		ImGui::Text("Change View Angle by left click drag");
		ImGui::Text("%2.2f estimated FPS", FPS);
		ImGui::Text("%1.4f normalized arc length", arcRatio);
		ImGui::Text("%1.4f speed (normalized arc length/time)", (speedtime.getSpeedFromDeltaTime(curveTime)));
		ImGui::Text("%1.4f lap time", curveTime);
		ImGui::Checkbox("DrawModel", &drawTriangle);
		ImGui::Checkbox("DrawFrame", &drawFrame);
		ImGui::Checkbox("DrawBone", &drawBone);
		ImGui::Checkbox("Pause Animation", &animPause);
		if (ImGui::SliderFloat("Accelerate Time", &speedtime.t1, 0.0f, speedtime.t2)) { speedtime.CalculateVmax(); curveTime = 0.0f; }
		if (ImGui::SliderFloat("MaxSpeed Time", &speedtime.t2, speedtime.t1, speedtime.tmax)) { speedtime.CalculateVmax(); curveTime = 0.0f; }
		if (ImGui::SliderFloat("Decelerate Time", &speedtime.tmax, speedtime.t2, 50.0f)) { speedtime.CalculateVmax(); curveTime = 0.0f; }
		
		for (int i = 0; i < CONTROL_POINT_COUNT; i++)
		{
			if (ImGui::SliderFloat3(("ControlPoint[" + std::to_string(i) + "]").c_str(), (float*)&spaceCurve.controlPoints[i], -5.0f, 5.0f))
			{
				curveTime = 0.0f; spaceCurve.UpdateSpaceCurve();
			}
		}
		//ImGui::SliderFloat3("ControlPoint[0]", (float*)&spaceCurve.controlPoints[0], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[1]", (float*)&spaceCurve.controlPoints[1], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[2]", (float*)&spaceCurve.controlPoints[2], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[3]", (float*)&spaceCurve.controlPoints[3], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[4]", (float*)&spaceCurve.controlPoints[4], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[5]", (float*)&spaceCurve.controlPoints[5], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[6]", (float*)&spaceCurve.controlPoints[6], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[7]", (float*)&spaceCurve.controlPoints[7], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[8]", (float*)&spaceCurve.controlPoints[8], -5.0f, 5.0f);
		//ImGui::SliderFloat3("ControlPoint[9]", (float*)&spaceCurve.controlPoints[9], -5.0f, 5.0f);
		//if (ImGui::Button("Recalculate Space Curve")) { curveTime = 0.0f; spaceCurve.UpdateSpaceCurve(); }


		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	shaderProgram.Delete();
	lightShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}