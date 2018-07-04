#include "stdafx.h"
#include "SceneUniform.h"

SceneUniform::SceneUniform()
:
m_model		(),
m_view		(),
m_projection()
{}

SceneUniform::SceneUniform
(
	Matrix4 const & model,
	Matrix4 const & view,
	Matrix4 const & projection
)
:
m_model		(model),
m_view		(view),
m_projection(projection)
{}

Matrix4 const & SceneUniform::getModel() const
{
	return m_model;
}

Matrix4 & SceneUniform::model()
{
	return const_cast<Matrix4 &>(static_cast<SceneUniform const &>(*this).getModel());
}

Matrix4 const & SceneUniform::getView() const
{
	return m_view;
}

Matrix4 & SceneUniform::view()
{
	return const_cast<Matrix4 &>(static_cast<SceneUniform const &>(*this).getView());
}

Matrix4 const & SceneUniform::getProjection() const
{
	return m_projection;
}

Matrix4 & SceneUniform::projection()
{
	return const_cast<Matrix4 &>(static_cast<SceneUniform const &>(*this).getProjection());
}