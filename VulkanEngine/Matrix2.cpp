#include "stdafx.h"
#include "Matrix2.h"

Matrix2::Matrix2()
{
	data[0] = Vector2();
	data[1] = Vector2();
}

Matrix2::Matrix2
(
	Vector2 const & v
)
{
	data[0] = v;
	data[1] = v;
}

Matrix2::Matrix2
(
	Vector2 const & a,
	Vector2 const & b
)
{
	data[0] = a;
	data[1] = b;
}

Matrix2::Matrix2
(
	float v_00,
	float v_01,
	float v_10,
	float v_11
)
{
	data[0] = Vector2(v_00, v_01);
	data[1] = Vector2(v_10, v_11);
}

Matrix2::Matrix2
(
	Matrix2 const & other
)
{
	*this = other;
}

Matrix2 & Matrix2::operator=
(
	Matrix2 const & other
)
{
	Vector2 const * otherData	= other.data;
	data[0]						= otherData[0];
	data[1]						= otherData[1];

	return *this;
}