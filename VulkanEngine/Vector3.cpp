#include "stdafx.h"
#include "Vector3.h"

Vector3::Vector3()
:
x(),
y(),
z()
{}

Vector3::Vector3
(
	float s
)
:
x(s),
y(s),
z(s)
{}

Vector3::Vector3
(
	float a,
	float b
)
:
x(a),
y(b),
z()
{}

Vector3::Vector3
(
	float a,
	float b,
	float c
)
:
x(a),
y(b),
z(c)
{}

Vector3::Vector3
(
	Vector2 const & v
)
:
x(v.x),
y(v.y),
z()
{}

Vector3::Vector3
(
	Vector3 const & other
)
:
x(other.x),
y(other.y),
z(other.z)
{}

float & Vector3::operator[](size_t index)
{
	return const_cast<float &>(static_cast<Vector3 const &>(*this)[index]);
}

float const & Vector3::operator[](size_t index) const
{
	if (index > 3)
	{
		throw std::out_of_range("Attempt to access Vector3 data with out of range offset");
	}

	return (&x)[index];
}

Vector3 & Vector3::operator=
(
	Vector3 const & other
)
{
	x = other.x;
	y = other.y;
	z = other.z;
	
	return *this;
}

Vector3 Vector3::operator*
(
	float scalar
) const
{
	return Vector3(scalar * x, scalar * y, scalar * z);
}

Vector3 Vector3::operator+
(
	Vector3 const & other
) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-
(
	Vector3 const & other
) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}
