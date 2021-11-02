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

	//initialize the space curve with the default control points
	//also initialize sizes of the vectors that gets passed to shader
	SpaceCurve();

	//Generates intermediate control points from the main control points
	void CreateIntermediateControlPoint();

	//Updates the data that gets passed to the shader vertices/indices
	void UpdateSpaceCurve();

	//The DeCastlejau alg that calculates the positions on the curve (100 between each 2 control points)
	glm::vec3 Interpolate(float input);

	//Creates a segment table by adaptive approach
	//Also normalizes the totalArclength to one after creating the whole table
	void createTable();

	//Returns the corresponding u of a given normalized arclength (binary search)
	float EvalArc(float input);

	//Returns the position of a given normalized arclength
	glm::vec3 constSpeedEval(float input);


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
