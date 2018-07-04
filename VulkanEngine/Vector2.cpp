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

float & Vector2::operator[](size_t index)
{
	return const_cast<float &>(static_cast<Vector2 const &>(*this)[index]);
}

float const & Vector2::operator[](size_t index) const
{
	if (index > 2)
	{
		throw std::out_of_range("Attempt to access Vector2 data with out of range offset");
	}

	return (&x)[index];
}

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
