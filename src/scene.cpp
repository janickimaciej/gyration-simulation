#include "scene.hpp"

#include "mesh.hpp"

#include <glad/glad.h>

#include <cmath>

static constexpr float nearPlane = 0.1f;
static constexpr float farPlane = 1000.0f;
static constexpr float initFOVYDeg = 60.0f;

static constexpr glm::vec3 cubeColor = {0, 0.5f, 1.0f};
static constexpr glm::vec3 hingeColor = {1.0f, 0, 0};
static constexpr float hingeSide = 0.05f;

static constexpr glm::vec3 diagonalColor = {0, 0.5f, 1.0f};
static constexpr glm::vec3 trajectoryColor = {1.0f, 1.0f, 1.0f};
static constexpr glm::vec3 gravityVectorColor = {1.0f, 0, 0};

Scene::Scene(const glm::ivec2& viewportSize) :
	m_camera{viewportSize, nearPlane, farPlane, initFOVYDeg},
	m_cube{cubeColor},
	m_hinge{hingeColor},
	m_diagonal{diagonalColor},
	m_trajectory{trajectoryColor},
	m_gravityVector{gravityVectorColor}
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	m_hinge.setPos({-hingeSide / 2.0f, -hingeSide / 2.0f, -hingeSide / 2.0f});
	m_hinge.setScale({hingeSide, hingeSide, hingeSide});
}

void Scene::update()
{
	m_simulation.update();
	glm::quat orientation = m_simulation.getOrientation();
	m_cube.setOrientation(orientation * glm::quat_cast(Simulation::initialRotation()));
	m_cube.setScale({m_simulation.getSide(), m_simulation.getSide(), m_simulation.getSide()});
	glm::vec3 diagonal = orientation * glm::vec3{0, m_simulation.getSide() * std::sqrt(3.0f), 0};
	m_diagonal.update({{0, 0, 0}, diagonal});
	m_trajectory.update(m_simulation.getTrajectory());
	glm::vec3 centerOfMass = diagonal / 2.0f;
	m_gravityVector.update(
		{
			centerOfMass,
			centerOfMass + glm::vec3{0, -1.5f, 0},
			centerOfMass + glm::vec3{-0.05f, -1.4f, 0},
			centerOfMass + glm::vec3{0.05f, -1.4f, 0},
			centerOfMass + glm::vec3{0, -1.5f, 0},
			centerOfMass + glm::vec3{0, -1.4f, -0.05f},
			centerOfMass + glm::vec3{0, -1.4f, 0.05f},
			centerOfMass + glm::vec3{0, -1.5f, 0}
		});
}

void Scene::render() const
{
	static constexpr glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera.use();

	if (m_renderDiagonal)
	{
		m_diagonal.render();
	}
	if (m_renderGravityVector)
	{
		m_gravityVector.render();
	}
	if (m_renderCube)
	{
		m_cube.render();
		m_hinge.render();
	}
	if (m_renderTrajectory)
	{
		m_trajectory.render();
	}
	if (m_renderPlane)
	{
		m_plane.render();
	}
}

void Scene::updateViewportSize()
{
	m_camera.updateViewportSize();
}

void Scene::addPitchCamera(float pitchRad)
{
	m_camera.addPitch(pitchRad);
}

void Scene::addYawCamera(float yawRad)
{
	m_camera.addYaw(yawRad);
}

void Scene::moveXCamera(float x)
{
	m_camera.moveX(x);
}

void Scene::moveYCamera(float y)
{
	m_camera.moveY(y);
}

void Scene::zoomCamera(float zoom)
{
	m_camera.zoom(zoom);
}

bool Scene::getRenderCube() const
{
	return m_renderCube;
}

void Scene::setRenderCube(bool renderCube)
{
	m_renderCube = renderCube;
}

bool Scene::getRenderDiagonal() const
{
	return m_renderDiagonal;
}

void Scene::setRenderDiagonal(bool renderDiagonal)
{
	m_renderDiagonal = renderDiagonal;
}

bool Scene::getRenderTrajectory() const
{
	return m_renderTrajectory;
}

void Scene::setRenderTrajectory(bool renderTrajectory)
{
	m_renderTrajectory = renderTrajectory;
}

bool Scene::getRenderGravityVector() const
{
	return m_renderGravityVector;
}

void Scene::setRenderGravityVector(bool renderGravityVector)
{
	m_renderGravityVector = renderGravityVector;
}

bool Scene::getRenderPlane() const
{
	return m_renderPlane;
}

void Scene::setRenderPlane(bool renderPlane)
{
	m_renderPlane = renderPlane;
}

Simulation& Scene::getSimulation()
{
	return m_simulation;
}
