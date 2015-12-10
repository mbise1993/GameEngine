#pragma once
#ifndef MATH_UTIL
#define MATH_UTIL

const float PI_CONST = 3.142159f;

#define DegToRad(degrees) ((degrees) * PI_CONST / 180.0f)

struct VertexData
{
	float x, y, z, w;
};

struct Vector3f
{
	float v1, v2, v3;

	void normalize();

	Vector3f getNormal();
};

struct Vector3s
{
	short v1, v2, v3;
};


#endif
