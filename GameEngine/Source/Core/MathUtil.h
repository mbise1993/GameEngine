#pragma once
#ifndef MATH_UTIL
#define MATH_UTIL

const float PI_CONST = 3.142159f;

#define DegToRad(degrees) ((degrees) * PI_CONST / 180.0f)

struct VertexData {
	float x, y, z, w;
};

struct Vector3f {
	float v1, v2, v3;
};

inline bool isBetween(float num1, float num2, float numToCheck)
{
	return num1 < num2 ? 
		(numToCheck > num1 && numToCheck < num2) : 
		(numToCheck > num2 && numToCheck < num1);
}

#endif
