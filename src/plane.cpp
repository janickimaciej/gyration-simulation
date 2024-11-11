#include "plane.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>

Plane::Plane(const ShaderProgram& shaderProgram) :
	m_shaderProgram{shaderProgram}
{
	createVBO();
	createEBO();
	createVAO();
}

Plane::~Plane()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteBuffers(1, &m_VBO);
}

void Plane::render() const
{
	m_shaderProgram.use();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(12), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Plane::createVBO()
{
	static constexpr std::array<glm::vec3, 4> vertices =
	{{
		{-1000, 0, -1000},
		{-1000, 0, 1000},
		{1000, 0, -1000},
		{1000, 0, 1000}
	}};

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3)),
		vertices.data(), GL_STATIC_DRAW);
}

void Plane::createEBO()
{
	static constexpr std::array<unsigned int, 12> indices =
	{{
		0, 1, 2,
		1, 2, 3,
		0, 2, 1,
		1, 3, 2
	}};

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(),
		GL_STATIC_DRAW);
}

void Plane::createVAO()
{
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBindVertexArray(0);
}
