#include "model.hpp"

void Model::setPosition(const glm::vec3& position)
{
	m_position = position;
	updateMatrix();
}

void Model::setOrientation(const glm::quat& orientation)
{
	m_orientation = orientation;
	updateMatrix();
}

void Model::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	updateMatrix();
}

glm::mat4 Model::getMatrix() const
{
	return m_modelMatrix;
}

void Model::updateMatrix()
{
	glm::mat4 scaleMatrix
	{
		m_scale.x, 0, 0, 0,
		0, m_scale.y, 0, 0,
		0, 0, m_scale.z, 0,
		0, 0, 0, 1
	};

	glm::mat4 orientationMatrix = glm::mat4_cast(m_orientation);

	glm::mat4 positionMatrix
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_position.x, m_position.y, m_position.z, 1
		};

	m_modelMatrix = positionMatrix * orientationMatrix * scaleMatrix;
}
