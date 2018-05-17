#pragma once

#include "stdafx.h"
#include "Matrix4.h"
#include "VectorTransform.h"

class MatrixTransform
{
public:

	static float toRadians
	(
		float degrees
	);

	static Matrix4 identityMatrix4();

	static Matrix4 view
	(
		Vector3 const & eye,
		Vector3 const & center,
		Vector3 const & up
	);

	static Matrix4 projection
	(
		float verticalFieldOfView,
		float aspectRatio,
		float zNear,
		float zFar
	);
};