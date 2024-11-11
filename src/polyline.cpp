#include "polyline.hpp"

Polyline::Polyline(const ShaderProgram& shaderProgram, const glm::vec3& color) :
	m_shaderProgram{shaderProgram},
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
	m_shaderProgram.use();
	m_shaderProgram.setUniform("color", m_color);
}
