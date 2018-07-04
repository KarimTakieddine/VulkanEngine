#include "stdafx.h"
#include "Vertex.h"

Vertex::Vertex()
:
position(),
color	()
{}

Vertex::Vertex
(
	Vector3 const & p
)
:
position(p),
color	(1.0f, 1.0f, 1.0f)
{}

Vertex::Vertex
(
	Vector3 const & p,
	Vector3 const & c
)
:
position(p),
color	(c)
{}

Vertex::Vertex
(
	Vertex const & other
)
:
position(other.position),
color	(other.color)
{}