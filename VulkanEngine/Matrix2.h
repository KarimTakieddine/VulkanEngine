#pragma once

#include "stdafx.h"
#include "Vector2.h"

struct Matrix2
{
	Vector2 data[2];

	Matrix2();

	Matrix2
	(
		Vector2 const & v
	);

	Matrix2
	(
		Vector2 const & a,
		Vector2 const & b
	);

	Matrix2
	(
		float v_00,
		float v_01,
		float v_10,
		float v_11
	);

	Matrix2
	(
		Matrix2 const & other
	);

	Matrix2 & operator=
	(
		Matrix2 const & other
	);
};