#pragma once

#include "model.hpp"
#include "shaderProgram.hpp"

#include <glm/glm.hpp>

#include <memory>

class Box : public Model
{
public:
	Box(const ShaderProgram& shaderProgram, const glm::vec3& color);
	virtual void render() const override;

private:
	std::shared_ptr<Mesh> m_mesh{};
	const ShaderProgram& m_shaderProgram;
	glm::vec3 m_color{};

	void updateShaders() const;
};
