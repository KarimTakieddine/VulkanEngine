#include "stdafx.h"
#include "VectorTransform.h"

float VectorTransform::dot
(
	Vector2 const & a,
	Vector2 const & b
)
{
	return (a.x * b.x) + (a.y * b.y);
}

float VectorTransform::dot
(
	Vector3 const & a,
	Vector3 const & b
)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float VectorTransform::dot
(
	Vector4 const & a,
	Vector4 const & b
)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

Vector3 VectorTransform::cross
(
	Vector3 const & a,
	Vector3 const & b
)
{
	float a_x = a.x;
	float a_y = a.y;
	float a_z = a.z;

	float b_x = b.x;
	float b_y = b.y;
	float b_z = b.z;

	return Vector3
	(
		(a_y * b_z) - (a_z * b_y),
		(a_z * b_x) - (a_x * b_z),
		(a_x * b_y) - (a_y * b_x)
	);
}