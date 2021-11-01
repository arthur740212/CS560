#include "spacecurve.h"

SpaceCurve::SpaceCurve()
{
	spaceCurveVertices.assign(interpolatePoints, Vertex());
	spaceCurveIndices.assign(2 * interpolatePoints, 0);
	controlPointVertices.assign(CONTROL_POINT_COUNT, Vertex());
	controlPointIndices.assign(CONTROL_POINT_COUNT, 0);
	UpdateSpaceCurve();
	createTable();
}

void SpaceCurve::CreateIntermediateControlPoint()
{
	for (int i = 0; i < CONTROL_POINT_COUNT; i++)
	{
		int left = (i + 4) % CONTROL_POINT_COUNT;
		int right = (i + 1) % CONTROL_POINT_COUNT;

		aPoints[i] = glm::mix(glm::mix(controlPoints[left], controlPoints[i], 2.0f), controlPoints[right], 0.5f);
		bPoints[i] = glm::mix(aPoints[i], controlPoints[i], 2.0f);
	}
}

void SpaceCurve::createTable()
{
	segTable.clear();
	segList.push_front(Pair{ 0.0f,CONTROL_POINT_COUNT, 0.0f });
	segTable.push_back(Pair{ 0.0f,0.0f, 0.0f });
	while (!segList.empty())
	{

		Pair pair = segList.front();

		float um = (pair.ua + pair.ub) / 2.0f;
		float A = glm::length(Interpolate(pair.ua) - Interpolate(um));
		float B = glm::length(Interpolate(pair.ub) - Interpolate(um));
		float C = glm::length(Interpolate(pair.ub) - Interpolate(pair.ua));
		float d = A + B - C;
		if (d > errorThreshold && pair.ub - pair.ua > maxInterval)
		{
			segList.pop_front();
			segList.push_front(Pair{ um,pair.ub,pair.length });
			segList.push_front(Pair{ pair.ua,um,pair.length });
		}
		else
		{
			segList.pop_front();
			float length = segTable.back().length;
			segTable.push_back(Pair{ pair.ua,um,length + A });
			segTable.push_back(Pair{ um,pair.ub,length + A + B });
		}
	}
	float totalArc = segTable.back().length;
	for (auto it = segTable.begin(); it != segTable.end(); it++)
	{
		it->length /= totalArc;
	}
}

float SpaceCurve::EvalArc(float input)
{
	int min = 0;
	int max = segTable.size();
	do
	{
		int half = (min + max) / 2;
		if (input > segTable[half].length)
		{
			min = half;
		}
		else
		{
			max = half;
		}
	} while (max - min > 1);

	float ua = segTable[max].ua;
	float ub = segTable[max].ub;
	float upperBound = segTable[max].length;
	float lowerBound = segTable[min].length;

	return ua + (ub - ua) / (upperBound - lowerBound) * (input - lowerBound);
}

glm::vec3 SpaceCurve::constSpeedEval(float input)
{
	return Interpolate(EvalArc(input));
}

void SpaceCurve::UpdateSpaceCurve()
{
	CreateIntermediateControlPoint();
	for (int i = 0; i < interpolatePoints; i++)
	{
		if (i < 100.0f * CONTROL_POINT_COUNT)
		{
			//spaceCurveVertices[i].position = Evaluate(i / 100.0f);
			spaceCurveVertices[i].position = Interpolate(i / 100.0f);
			spaceCurveVertices[i].color = glm::vec3(1.0f, 0.0f, i / (100.0f * CONTROL_POINT_COUNT));
		}
		else
		{
			spaceCurveVertices[i].position = glm::vec3(0);
		}
	}
	createTable();
	for (int i = 0; i < interpolatePoints * 2 - 1; i++)
	{
		spaceCurveIndices[i] = (i + 1) / 2;
	}
	spaceCurveIndices[1999] = 0;

	for (int i = 0; i < CONTROL_POINT_COUNT; i++)
	{
		controlPointVertices[i].position = controlPoints[i];
		controlPointVertices[i].color = glm::vec3(0.0f, 0.0f, (float)i / CONTROL_POINT_COUNT);
		controlPointIndices[i] = i;
	}
}

glm::vec3 SpaceCurve::Interpolate(float input)
{
	int floorT = (int)input;
	float t = input - floorT;
	deCas[0] = controlPoints[floorT];
	deCas[1] = aPoints[floorT];
	deCas[2] = bPoints[(floorT + 1) % CONTROL_POINT_COUNT];
	deCas[3] = controlPoints[(floorT + 1) % CONTROL_POINT_COUNT];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3 - i; j++)
		{
			deCas[j] = glm::mix(deCas[j], deCas[j + 1], t);
		}
	}
	return deCas[0];
}

