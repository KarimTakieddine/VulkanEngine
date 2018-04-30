#pragma once
#include "stdafx.h"
#include "Vector3.h"

struct Vertex
{
	Vector3 position, color;

	Vertex();

	Vertex
	(
		Vector3 const & p
	);
	
	Vertex
	(
		Vector3 const & p,
		Vector3 const & c
	);

	Vertex
	(
		Vertex const & other
	);
};