#include "stdafx.h"
#include "Matrix4.h"

Matrix4::Matrix4()
{
	data[0] = Vector4();
	data[1] = Vector4();
	data[2] = Vector4();
	data[3] = Vector4();
}

Matrix4::Matrix4
(
	Vector4 const & v
)
{
	data[0] = v;
	data[1] = v;
	data[2] = v;
	data[3] = v;
}

Matrix4::Matrix4
(
	Vector4 const & a,
	Vector4 const & b
)
{
	data[0] = a;
	data[1] = b;
	data[2] = Vector4();
	data[3] = Vector4();
}

Matrix4::Matrix4
(
	Vector4 const & a,
	Vector4 const & b,
	Vector4 const & c
)
{
	data[0] = a;
	data[1] = b;
	data[2] = c;
	data[3] = Vector4();
}

Matrix4::Matrix4
(
	Vector4 const & a,
	Vector4 const & b,
	Vector4 const & c,
	Vector4 const & d
)
{
	data[0] = a;
	data[1] = b;
	data[2] = c;
	data[3] = d;
}

Matrix4::Matrix4
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
)
{
	data[0] = Vector4(v_00, v_01, v_02, v_03);
	data[1] = Vector4(v_10, v_11, v_12, v_13);
	data[2] = Vector4(v_20, v_21, v_22, v_23);
	data[3] = Vector4(v_30, v_31, v_32, v_33);
}

Matrix4::Matrix4
(
	Matrix4 const & other
)
{
	*this = other;
}

Vector4 & Matrix4::operator[](size_t index)
{
	return const_cast<Vector4 &>(static_cast<Matrix4 const &>(*this)[index]);
}

Vector4 const & Matrix4::operator[](size_t index) const
{
	if (index > 4)
	{
		throw std::out_of_range("Attempt to access Matrix4 data with out of range offset");
	}

	return data[index];
}

Matrix4 & Matrix4::operator=
(
	Matrix4 const & other
)
{
	Vector4 const * otherData	= other.data;
	data[0]						= otherData[0];
	data[1]						= otherData[1];
	data[2]						= otherData[2];
	data[3]						= otherData[3];

	return *this;
}

Matrix4 operator*
(
	Matrix4 const & a,
	Matrix4 const & b
)
{
	return Matrix4
	(
		Vector4
		(
			VectorTransform::dot(a[0], Vector4(b[0][0], b[1][0], b[2][0], b[3][0])),
			VectorTransform::dot(a[0], Vector4(b[0][1], b[1][1], b[2][1], b[3][1])),
			VectorTransform::dot(a[0], Vector4(b[0][2], b[1][2], b[2][2], b[3][2])),
			VectorTransform::dot(a[0], Vector4(b[0][3], b[1][3], b[2][3], b[3][3]))
		),

		Vector4
		(
			VectorTransform::dot(a[1], Vector4(b[0][0], b[1][0], b[2][0], b[3][0])),
			VectorTransform::dot(a[1], Vector4(b[0][1], b[1][1], b[2][1], b[3][1])),
			VectorTransform::dot(a[1], Vector4(b[0][2], b[1][2], b[2][2], b[3][2])),
			VectorTransform::dot(a[1], Vector4(b[0][3], b[1][3], b[2][3], b[3][3]))
		),

		Vector4
		(
			VectorTransform::dot(a[2], Vector4(b[0][0], b[1][0], b[2][0], b[3][0])),
			VectorTransform::dot(a[2], Vector4(b[0][1], b[1][1], b[2][1], b[3][1])),
			VectorTransform::dot(a[2], Vector4(b[0][2], b[1][2], b[2][2], b[3][2])),
			VectorTransform::dot(a[2], Vector4(b[0][3], b[1][3], b[2][3], b[3][3]))
		),

		Vector4
		(
			VectorTransform::dot(a[3], Vector4(b[0][0], b[1][0], b[2][0], b[3][0])),
			VectorTransform::dot(a[3], Vector4(b[0][1], b[1][1], b[2][1], b[3][1])),
			VectorTransform::dot(a[3], Vector4(b[0][2], b[1][2], b[2][2], b[3][2])),
			VectorTransform::dot(a[3], Vector4(b[0][3], b[1][3], b[2][3], b[3][3]))
		)
	);
}