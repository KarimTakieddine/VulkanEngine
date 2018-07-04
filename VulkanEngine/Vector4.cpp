#include "stdafx.h"
#include "Vector4.h"

Vector4::Vector4()
:
x(),
y(),
z(),
w()
{}

Vector4::Vector4
(
	float s
)
:
x(s),
y(s),
z(s),
w(s)
{}

Vector4::Vector4
(
	float a,
	float b
)
:
x(a),
y(b),
z(),
w()
{}

Vector4::Vector4
(
	float a,
	float b,
	float c
)
:
x(a),
y(b),
z(c),
w()
{}

Vector4::Vector4
(
	float a,
	float b,
	float c,
	float d
)
:
x(a),
y(b),
z(c),
w(d)
{}

Vector4::Vector4
(
	Vector2 const & v
)
:
x(v.x),
y(v.y),
z(),
w()
{}

Vector4::Vector4
(
	Vector3 const & v
)
:
x(v.x),
y(v.y),
z(v.z),
w()
{}

Vector4::Vector4
(
	Vector4 const & other
)
:
x(other.x),
y(other.y),
z(other.z),
w(other.w)
{}

float & Vector4::operator[](size_t index)
{
	return const_cast<float &>(static_cast<Vector4 const &>(*this)[index]);
}

float const & Vector4::operator[](size_t index) const
{
	if (index > 4)
	{
		throw std::out_of_range("Attempt to access Vector4 data with out of range offset");
	}

	return (&x)[index];
}

Vector4 & Vector4::operator=
(
	Vector4 const & other
)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;

	return *this;
}

Vector4 Vector4::operator*
(
	float scalar
) const
{
	return Vector4(scalar * x, scalar * y, scalar * z, scalar * w);
}

Vector4 Vector4::operator+
(
	Vector4 const & other
) const
{
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-
(
	Vector4 const & other
) const
{
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}
