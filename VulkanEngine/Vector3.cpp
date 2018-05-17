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
