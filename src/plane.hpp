#pragma once

class Plane
{
public:
	Plane();
	~Plane();

	void render() const;

private:
	unsigned int m_VBO{};
	unsigned int m_EBO{};
	unsigned int m_VAO{};

	void createVBO();
	void createEBO();
	void createVAO();
};
