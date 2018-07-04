#pragma once
#include "stdafx.h"
#include "Vector2.h"

struct Vector3
{
	float x, y, z;

	Vector3();

	Vector3
	(
		float s
	);

	Vector3
	(
		float a,
		float b
	);

	Vector3
	(
		float a,
		float b,
		float c
	);

	Vector3
	(
		Vector2 const & v
	);

	Vector3
	(
		Vector3 const & other
	);

	float & operator[]
	(
		size_t index
	);

	float const & operator[]
	(
		size_t index
	) const;

	Vector3 & operator=
	(
		Vector3 const & other
	);

	Vector3 operator*
	(
		float scalar
	) const;

	Vector3 operator+
	(
		Vector3 const & other	
	) const;

	Vector3 operator-
	(
		Vector3 const & other	
	) const;
};