#pragma once

#include "stdafx.h"
#include "VectorTransform.h"

struct Matrix4
{
	Vector4 data[4];

	Matrix4();

	Matrix4
	(
		Vector4 const & v
	);

	Matrix4
	(
		Vector4 const & a,
		Vector4 const & b
	);

	Matrix4
	(
		Vector4 const & a,
		Vector4 const & b,
		Vector4 const & c
	);

	Matrix4
	(
		Vector4 const & a,
		Vector4 const & b,
		Vector4 const & c,
		Vector4 const & d
	);

	Matrix4
	(
		float v_00,
		float v_01,
		float v_02,
		float v_03,
		float v_10,
		float v_11,
		float v_12,
		float v_13,
		float v_20,
		float v_21,
		float v_22,
		float v_23,
		float v_30,
		float v_31,
		float v_32,
		float v_33
	);

	Matrix4
	(
		Matrix4 const & other
	);

	Vector4 & operator[]
	(
		size_t index
	);

	Vector4 const & operator[]
	(
		size_t index
	) const;

	Matrix4 & operator=
	(
		Matrix4 const & other
	);
};

Matrix4 operator*(Matrix4 const & a, Matrix4 const & b);