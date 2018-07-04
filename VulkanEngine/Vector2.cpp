#include "stdafx.h"
#include "Vector2.h"

Vector2::Vector2()
:
x(),
y()
{}

Vector2::Vector2
(
	float s
)
:
x(s),
y(s)
{}

Vector2::Vector2
(
	float a,
	float b
)
:
x(a),
y(b)
{}

Vector2::Vector2
(
	Vector2 const & other
)
:
x(other.x),
y(other.y)
{}

Vector2 & Vector2::operator=
(
	Vector2 const & other
)
{
	x = other.x;
	y = other.y;
	
	return *this;
}

Vector2 Vector2::operator*
(
	float scalar
) const
{
	return Vector2(scalar * x, scalar * y);
}

Vector2 Vector2::operator+
(
	Vector2 const & other
) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-
(
	Vector2 const & other
) const
{
	return Vector2(x - other.x, y - other.y);
}
