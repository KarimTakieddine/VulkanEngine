#pragma once

#include "Vector4.h"

class VectorTransform
{
	static float dot
	(
		Vector2 const & a,
		Vector2 const & b
	);

	static float dot
	(
		Vector3 const & a,
		Vector3 const & b
	);

	static float dot
	(
		Vector4 const & a,
		Vector4 const & b
	);
	
	static Vector3 cross
	(
		Vector3 const & a,
		Vector3 const & b
	);
};