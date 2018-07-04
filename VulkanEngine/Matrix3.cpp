#include "stdafx.h"
#include "Matrix3.h"

Matrix3::Matrix3()
{
	data[0] = Vector3();
	data[1] = Vector3();
	data[2] = Vector3();
}

Matrix3::Matrix3
(
	Vector3 const & v
)
{
	data[0] = v;
	data[1] = v;
	data[2] = v;
}

Matrix3::Matrix3
(
	Vector3 const & a,
	Vector3 const & b
)
{
	data[0] = a;
	data[1] = b;
	data[2] = Vector3();
}

Matrix3::Matrix3
(
	Vector3 const & a,
	Vector3 const & b,
	Vector3 const & c
)
{
	data[0] = a;
	data[1] = b;
	data[2] = c;
}

Matrix3::Matrix3
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
)
{
	data[0] = Vector3(v_00, v_01, v_02);
	data[1] = Vector3(v_10, v_11, v_12);
	data[2] = Vector3(v_20, v_21, v_22);
}

Matrix3::Matrix3
(
	Matrix3 const & other
)
{
	*this = other;
}

Matrix3 & Matrix3::operator=
(
	Matrix3 const & other
)
{
	Vector3 const * otherData	= other.data;
	data[0]						= otherData[0];
	data[1]						= otherData[1];
	data[2]						= otherData[2];

	return *this;
}