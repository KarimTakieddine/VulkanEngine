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

	Vector2 & operator=
	(
		Vector2 const & other
	);

	Vector2 operator*
	(
		float scalar
	) const;
};