#include "polyline.hpp"

#include "shaderPrograms.hpp"

Polyline::Polyline(const glm::vec3& color) :
	m_polyline{{}},
	m_color{color}
{ }

void Polyline::update(const std::vector<glm::vec3>& vertices)
{
	m_polyline.update(vertices);
}

void Polyline::render() const
{
	updateShaders();
	m_polyline.render();
}

void Polyline::updateShaders() const
{
	ShaderPrograms::polyline->use();
	ShaderPrograms::polyline->setUniform("color", m_color);
}
