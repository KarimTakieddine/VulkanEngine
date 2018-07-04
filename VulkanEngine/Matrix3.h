#pragma once

#include "stdafx.h"
#include "Vector3.h"

struct Matrix3
{
	Vector3 data[3];

	Matrix3();

	Matrix3
	(
		Vector3 const & v
	);

	Matrix3
	(
		Vector3 const & a,
		Vector3 const & b
	);

	Matrix3
	(
		Vector3 const & a,
		Vector3 const & b,
		Vector3 const & c
	);

	Matrix3
	(
		float v_00,
		float v_01,
		float v_02,
		float v_10,
		float v_11,
		float v_12,
		float v_20,
		float v_21,
		float v_22
	);

	Matrix3
	(
		Matrix3 const & other
	);

	Vector3 & operator[]
	(
		size_t index
	);

	Vector3 const & operator[]
	(
		size_t index
	) const;

	Matrix3 & operator=
	(
		Matrix3 const & other
	);
};