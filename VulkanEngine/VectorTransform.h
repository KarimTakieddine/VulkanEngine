#pragma once

#include "Vector4.h"

class VectorTransform
{
	static float magnitude
	(
		Vector2 const & v
	);

	static float magnitude
	(
		Vector3 const & v
	);

	static float magnitude
	(
		Vector4 const & v
	);

	static Vector2 normalized
	(
		Vector2 const & v
	);

	static Vector3 normalized
	(
		Vector3 const & v
	);

	static Vector4 normalized
	(
		Vector4 const & v
	);

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