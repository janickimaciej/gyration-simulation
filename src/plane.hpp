#pragma once

#include "shaderProgram.hpp"

class Plane
{
public:
	Plane(const ShaderProgram& shaderProgram);
	~Plane();

	void render() const;

private:
	const ShaderProgram& m_shaderProgram;

	unsigned int m_VBO{};
	unsigned int m_EBO{};
	unsigned int m_VAO{};

	void createVBO();
	void createEBO();
	void createVAO();
};
