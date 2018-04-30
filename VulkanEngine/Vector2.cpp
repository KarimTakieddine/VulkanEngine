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