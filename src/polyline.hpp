#pragma once

#include "polylineMesh.hpp"

#include <glm/glm.hpp>

#include <vector>

class Polyline
{
public:
	Polyline(const glm::vec3& color);
	void update(const std::vector<glm::vec3>& vertices);
	void render() const;

private:
	PolylineMesh m_polyline;
	glm::vec3 m_color{};

	void updateShaders() const;
};
