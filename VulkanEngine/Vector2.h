#pragma once
#include "stdafx.h"

struct Vector2
{
	float x, y;

	Vector2();

	Vector2
	(
		float s
	);

	Vector2
	(
		float a,
		float b
	);

	Vector2
	(
		Vector2 const & other
	);

	float & operator[]
	(
		size_t index
	);

	float const & operator[]
	(
		size_t index
	) const;

	Vector2 & operator=
	(
		Vector2 const & other
	);

	Vector2 operator*
	(
		float scalar
	) const;

	Vector2 operator+
	(
		Vector2 const & other
	) const;

	Vector2 operator-
	(
		Vector2 const & other
	) const;
};