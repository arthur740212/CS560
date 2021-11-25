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


#pragma region previousstuff
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

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Proj3", NULL, NULL);
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
	
	//Create Target Mesh
	Mesh target(lightVerts, lightInd, tex);

	glm::vec4 targetColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec3 targetPos = glm::vec3(-0.5f, 0.5f, 0.5f);
	glm::mat4 targetModel = glm::mat4(1.0f);
	targetModel = glm::translate(targetModel, targetPos);

	//Pyramid
	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	glm::mat4 modelSize = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.02f, 0.02f));
	glm::mat4 modelOrient = glm::mat4(1.0f);
	glm::mat4 modelPos = glm::mat4(1.0f);

	glm::mat4 floorSize = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));

	
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Shader ColorShader("defaultColor.vert", "defaultColor.frag");
	ColorShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	
	Shader CurveShader("SpaceCurve.vert", "SpaceCurve.frag");
	CurveShader.Activate();

	Shader FloorShader("light.vert", "default.frag");
	FloorShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(FloorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorSize));
	glUniform4f(glGetUniformLocation(FloorShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(FloorShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	bool drawTriangle = true;
	bool drawFrame = false;
	bool drawBone = true;
	bool animPause = false;
	float rotation = 0.0f;

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 2.0f, 10.0f));

	float onCurveUnit = 0.0f;
	float arcRatio = 0.0f;
	float facingUnit = 0.0f;

	float curveTime = 0.0f;
	SpeedTime speedtime(10.0f, 24.0f, 30.0f);

	float animPace = 7.0f;
	float zeroAnimSpeed = 0.03f;

	bool isApproach = true;
	glm::vec3 curPos = glm::vec3(0.0f);
	std::string priorString = anim1.SetPriorityString();

	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float FPS = 1 / deltaTime;

		//curveTime += deltaTime;
		//if (curveTime >= speedtime.tmax) { curveTime -= speedtime.tmax; }
		//arcRatio = speedtime.getArcRatioFromDeltaTime(curveTime);
		//
		//if (!animPause)
		//{
		//	animor1.UpdateAnimation((speedtime.getSpeedFromDeltaTime(curveTime) + zeroAnimSpeed) / animPace);
		//}

		glClearColor(0.7f, 0.7f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#pragma region Shaderstuff
		if (!io.WantCaptureMouse)
		{
			//application input handling
			camera.Inputs(window);
		}
		shaderProgram.Activate();

		camera.UpdateMatrix(45.0f, 0.01f, 1000.0f);
		camera.Matrix(shaderProgram, "camMatrix");

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

		FloorShader.Activate();
		camera.Matrix(FloorShader, "camMatrix");
		glUniform3f(glGetUniformLocation(FloorShader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
		floor.Draw(FloorShader, camera);

		ColorShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelSize));
		glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, "modelPos"), 1, GL_FALSE, glm::value_ptr(modelPos));
		glUniformMatrix4fv(glGetUniformLocation(ColorShader.ID, "modelOrient"), 1, GL_FALSE, glm::value_ptr(modelOrient));

		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		light.Draw(lightShader, camera);

		targetModel = glm::translate(glm::mat4(1.0f), targetPos);

		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(targetModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), targetColor.x, targetColor.y, targetColor.z, targetColor.w);
		target.Draw(lightShader, camera);
#pragma endregion
		//if (drawTriangle)
		//{
		/*	onCurveUnit = spaceCurve.EvalArc(arcRatio);
			facingUnit = onCurveUnit + 0.1f;
			if (facingUnit > CONTROL_POINT_COUNT) { facingUnit -= CONTROL_POINT_COUNT; }
			glm::vec3 curPos = spaceCurve.Interpolate(onCurveUnit);
			modelPos = glm::translate(glm::mat4(1.0f), curPos);
			modelOrient = glm::inverse(glm::lookAt(spaceCurve.Interpolate(facingUnit), curPos, glm::vec3(0.0f, 1.0f, 0.0f)));*/
#pragma endregion

#pragma region Project3 is here (in the render loop)

		//Gets targets pos on xz plane
		glm::vec3 horizonTargetPos = targetPos;
		horizonTargetPos.y = 0.0f;


		if (isApproach)
		{
			//approach target when we press Move Towards Target Button, faces the targets
			//Stops when close enough
			if (glm::length(horizonTargetPos - curPos) < 0.5f) { isApproach = false; }
			else 
			{
				curPos = glm::normalize(horizonTargetPos - curPos) * deltaTime + curPos;
				modelPos = modelPos = glm::translate(glm::mat4(1.0f), curPos);
				modelOrient = glm::inverse(glm::lookAt(horizonTargetPos, curPos, glm::vec3(0.0f, 1.0f, 0.0f)));
				animor1.UpdateAnimation(deltaTime);
			}
		}


		else 
		{
			//When model not moving, the links point toward the target  
			modelOrient = glm::inverse(glm::lookAt(horizonTargetPos, curPos, glm::vec3(0.0f, 1.0f, 0.0f)));

			VQS ModelInverseVQS;
			modelPos* modelSize* modelOrient;
			ModelInverseVQS.decomposeMtx(modelPos * modelSize * modelOrient);

			//target in world tranferred to target in model space
			//Then calculate IK
			animor1.DoIK(ModelInverseVQS.Inverse() * targetPos);
		}

#pragma endregion
#pragma region previousstuff
#pragma region Display	
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
			//model1.meshes[i].Draw(shaderProgram, camera);
			glClear(GL_DEPTH_BUFFER_BIT);
			if (drawBone)
			{
				skeleton.DrawPoint(ColorShader, camera);
				skeletonLine.DrawLine(ColorShader, camera);
			}

		}
#pragma endregion	
		//}
		ColorShader.Activate();
		//spaceCurve.UpdateSpaceCurve();
		//spaceCurveLine.RebindVertices(spaceCurve.spaceCurveVertices);
		//spaceCurveControlPoints.RebindVertices(spaceCurve.controlPointVertices);
		//spaceCurveLine.DrawLine(CurveShader, camera);
		//spaceCurveControlPoints.DrawPoint(CurveShader, camera);

		ImGui::Begin("Hello Imgui");
		ImGui::Text("WASD to move, SHIFT to speed up");
		ImGui::Text("Change View Angle by left click drag");
		ImGui::Text("%2.2f estimated FPS", FPS);
		/*ImGui::Text("%1.4f normalized arc length", arcRatio);
		ImGui::Text("%1.4f speed (normalized arc length/time)", (speedtime.getSpeedFromDeltaTime(curveTime)));
		ImGui::Text("%1.4f lap time", curveTime);*/
		//ImGui::Checkbox("DrawModel", &drawTriangle);
		//ImGui::Checkbox("DrawFrame", &drawFrame);
		ImGui::Checkbox("DrawBone", &drawBone);
		//ImGui::Checkbox("Pause Animation", &animPause);
		/*ImGui::SliderFloat("Animation Pace", &animPace, 6.0f, 20.0f);
		ImGui::SliderFloat("Animation Speed at zero velocity ", &zeroAnimSpeed, 0.0f, 0.1f);*/
		if (ImGui::SliderFloat3("IK Targets Position", (float*)&targetPos, -3.0f, 3.0f));
		if (ImGui::Button("Move Towards Target")) { isApproach = true; }
		if (ImGui::Button("Default Priority")) { anim1.SetDefaultPriority(); animor1.GetManipFromAnimation(); priorString = anim1.SetPriorityString(); }
		if (ImGui::Button("Random Priority")) { anim1.SetNewPriority(); animor1.GetManipFromAnimation(); priorString = anim1.SetPriorityString();}
		ImGui::Text("Priority List (Parent Layer With Respect to End Effector)");
		ImGui::Text(priorString.c_str());
		
		/*if (ImGui::SliderFloat("Accelerate Time", &speedtime.t1, 0.0f, speedtime.t2)) { speedtime.CalculateVmax(); curveTime = 0.0f; }
		if (ImGui::SliderFloat("MaxSpeed Time", &speedtime.t2, speedtime.t1, speedtime.tmax)) { speedtime.CalculateVmax(); curveTime = 0.0f; }
		if (ImGui::SliderFloat("Decelerate Time", &speedtime.tmax, speedtime.t2, 50.0f)) { speedtime.CalculateVmax(); curveTime = 0.0f; }*/

		/*for (int i = 0; i < CONTROL_POINT_COUNT; i++)
		{
			if (ImGui::SliderFloat3(("ControlPoint[" + std::to_string(i) + "]").c_str(), (float*)&spaceCurve.controlPoints[i], -5.0f, 5.0f))
			{
				curveTime = 0.0f; spaceCurve.UpdateSpaceCurve();
			}
		}*/
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
#pragma endregion