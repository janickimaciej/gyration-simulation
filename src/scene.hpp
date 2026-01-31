#pragma once

#include "box.hpp"
#include "camera/perspectiveCamera.hpp"
#include "plane.hpp"
#include "polyline.hpp"
#include "simulation.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Scene
{
public:
	Scene(const glm::ivec2& viewportSize);
	void update();
	void render() const;
	void updateViewportSize();

	void addPitchCamera(float pitchRad);
	void addYawCamera(float yawRad);
	void moveXCamera(float x);
	void moveYCamera(float y);
	void zoomCamera(float zoom);

	bool getRenderCube() const;
	void setRenderCube(bool renderCube);
	bool getRenderDiagonal() const;
	void setRenderDiagonal(bool renderDiagonal);
	bool getRenderTrajectory() const;
	void setRenderTrajectory(bool renderTrajectory);
	bool getRenderGravityVector() const;
	void setRenderGravityVector(bool renderGravityVector);
	bool getRenderPlane() const;
	void setRenderPlane(bool renderPlane);

	Simulation& getSimulation();

private:
	const glm::ivec2& m_viewportSize{};
	PerspectiveCamera m_camera;

	Box m_cube;
	Box m_hinge;
	Polyline m_diagonal;
	Polyline m_trajectory;
	Polyline m_gravityVector;
	Plane m_plane{};

	bool m_renderCube = true;
	bool m_renderDiagonal = false;
	bool m_renderTrajectory = true;
	bool m_renderGravityVector = false;
	bool m_renderPlane = true;

	Simulation m_simulation{};

	void setAspectRatio(float aspectRatio);
};
