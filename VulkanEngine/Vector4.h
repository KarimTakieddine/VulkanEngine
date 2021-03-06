#pragma once

#include "Vector3.h"

struct Vector4
{
	float x, y, z, w;

	Vector4();

	Vector4
	(
		float s
	);

	Vector4
	(
		float a,
		float b
	);

	Vector4
	(
		float a,
		float b,
		float c
	);

	Vector4
	(
		float a,
		float b,
		float c,
		float d
	);

	Vector4
	(
		Vector2 const & v
	);

	Vector4
	(
		Vector3 const & v
	);

	Vector4
	(
		Vector4 const & other
	);

	float & operator[]
	(
		size_t index
	);

	float const & operator[]
	(
		size_t index
	) const;

	Vector4 & operator=
	(
		Vector4 const & other
	);

	Vector4 operator*
	(
		float scalar
	) const;

	Vector4 operator+
	(
		Vector4 const & other
	) const;

	Vector4 operator-
	(
		Vector4 const & other
	) const;
};