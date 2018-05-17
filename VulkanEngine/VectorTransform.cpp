#include "stdafx.h"
#include "VectorTransform.h"

float VectorTransform::magnitude
(
	Vector2 const & v
)
{
	float x = v.x;
	float y = v.y;

	return std::sqrtf((x * x) + (y * y));
}

float VectorTransform::magnitude
(
	Vector3 const & v
)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;

	return std::sqrtf((x * x) + (y * y) + (z * z));
}

float VectorTransform::magnitude
(
	Vector4 const & v
)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	float w = v.w;

	return std::sqrtf((x * x) + (y * y) + (z * z) + (w * w));
}

Vector2 VectorTransform::normalized
(
	Vector2 const & v
)
{
	return v * (1.0f / magnitude(v));
}

Vector3 VectorTransform::normalized
(
	Vector3 const & v
)
{
	return v * (1.0f / magnitude(v));
}

Vector4 VectorTransform::normalized
(
	Vector4 const & v
)
{
	return v * (1.0f / magnitude(v));
}

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