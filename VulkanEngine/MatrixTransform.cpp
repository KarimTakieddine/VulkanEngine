#include "stdafx.h"
#include "MatrixTransform.h"

float MatrixTransform::toRadians
(
	float degrees
)
{
	return degrees * (M_PI / 180.0f);
}

Matrix4 MatrixTransform::identityMatrix4()
{
	return Matrix4
	(
		Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4 MatrixTransform::view
(
	Vector3 const & eye,
	Vector3 const & center,
	Vector3 const & up
)
{
	Vector3 zAxis = VectorTransform::normalized(center - eye);
	Vector3 xAxis = VectorTransform::normalized(VectorTransform::cross(zAxis, up));
	Vector3 yAxis = VectorTransform::cross(xAxis, zAxis);

	return Matrix4
	(
		Vector4(xAxis.x,							yAxis.x,							-zAxis.x,							0.0f),
		Vector4(xAxis.y,							yAxis.y,							-zAxis.y,							0.0f),
		Vector4(xAxis.z,							yAxis.z,							-zAxis.z,							0.0f),
		Vector4(-VectorTransform::dot(xAxis, eye),	-VectorTransform::dot(yAxis, eye),	VectorTransform::dot(zAxis, eye),	1.0f)
	);
}

Matrix4 MatrixTransform::projection
(
	float verticalFieldOfView,
	float aspectRatio,
	float zNear,
	float zFar
)
{
	float const tanHalfFieldOfView = std::tanf(toRadians(verticalFieldOfView * 0.5f));

	return Matrix4
	(
		Vector4(1.0f / (aspectRatio * tanHalfFieldOfView),	0.0f,						0.0f,										0.0f),
		Vector4(0.0f,										1.0f / tanHalfFieldOfView,	0.0f,										0.0f),
		Vector4(0.0f,										0.0f,						-(zFar + zNear) / (zFar - zNear),			-1.0f),
		Vector4(0.0f, 										0.0f,						-(2.0f * zFar * zNear) / (zFar - zNear),	0.0f)
	);
}

Matrix4 MatrixTransform::rotation(float degrees)
{
	float radians = toRadians(degrees);

	return Matrix4
	(
		Vector4(std::cosf(radians), 0.0f, std::sinf(radians), 0.0f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4(-std::sinf(radians), 0.0f, std::cosf(radians), 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}
