#ifndef SPACECURVE_H
#define SPACECURVE_H

#include "model.h"
#include <list>

#define CONTROL_POINT_COUNT 10

struct Pair
{
	float ua;
	float ub;
	float length;
};


class SpaceCurve
{
public:
	
	glm::vec3 controlPoints[CONTROL_POINT_COUNT]
	{
		glm::vec3(-2.3f,0.0f,-5.0f),
		glm::vec3(-4.0f,0.0f,-1.5f),
		glm::vec3(-4.5f,0.0f,1.7f),
		glm::vec3(-2.3f,0.0f,5.0f),
		glm::vec3(3.3f,0.0f,5.0f),
		glm::vec3(5.0f,0.0f,3.5f),
		glm::vec3(-1.3f,0.0f,-1.3f),
		glm::vec3(1.6f,0.0f,-2.7f),
		glm::vec3(3.6f,0.0f,0.5f),
		glm::vec3(4.5f,0.0f,-5.0f),
	};

	glm::vec3 deCas[4];
	glm::vec3 aPoints[CONTROL_POINT_COUNT];
	glm::vec3 bPoints[CONTROL_POINT_COUNT];


	SpaceCurve();

	void CreateIntermediateControlPoint();
	void createTable();

	float EvalArc(float input);
	glm::vec3 constSpeedEval(float input);

	void UpdateSpaceCurve();

	glm::vec3 Interpolate(float input);


public:
	int interpolatePoints = 1000;
	std::vector<Vertex> spaceCurveVertices;
	std::vector<GLuint> spaceCurveIndices;

	std::vector<Vertex> controlPointVertices;
	std::vector<GLuint> controlPointIndices;

	std::list<Pair> segList;
	std::vector<Pair> segTable;

	float errorThreshold = 0.01f;
	float maxInterval = 0.01f;
};
#endif
