#pragma once

#include "stdafx.h"
#include "Matrix4.h"

struct SceneUniform
{
	Matrix4 m_model, m_view, m_projection;

	SceneUniform();

	SceneUniform
	(
		Matrix4 const & model,
		Matrix4 const & view,
		Matrix4 const & projection
	);

	Matrix4 & model();

	Matrix4 const & getModel() const;

	Matrix4 & view();

	Matrix4 const & getView() const;

	Matrix4 & projection();

	Matrix4 const & getProjection() const;
};