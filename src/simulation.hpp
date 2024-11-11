#pragma once

#include "state.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstddef>
#include <chrono>
#include <functional>
#include <vector>

class Simulation
{
public:
	Simulation();
	void update();
	void stop();
	void start();

	static glm::mat3 initialRotation();

	float getDT() const;
	void setDT(float dT);
	float getDensity() const;
	void setDensity(float density);
	float getSide() const;
	void setSide(float side);
	float getAngle0() const;
	void setAngle0(float angle0);
	float getAngVelocity0() const;
	void setAngVelocity0(float angVelocity0);
	std::size_t getTrajectoryLength() const;
	void setTrajectoryLength(std::size_t trajectoryLength);
	bool getGravity() const;
	void setGravity(bool gravity);

	int getIterations() const;
	float getT() const;
	glm::quat getOrientation() const;
	const std::vector<glm::vec3> getTrajectory() const;

private:
	bool m_running = false;

	float m_dT = 0.001f;
	float m_density = 1.0f;
	float m_side = 1.0f;
	float m_mass = {};
	float m_angle0 = glm::radians(15.0f);
	float m_angVelocity0 = 100.0f;
	glm::mat3 m_inertiaTensor{};
	glm::mat3 m_inertiaTensorInverse{};
	bool m_gravity = true;
	std::size_t m_trajectoryLength = 10000;

	State m_state{};

	std::chrono::time_point<std::chrono::system_clock> m_t0{};
	std::vector<float> m_t{};
	std::vector<glm::vec3> m_trajectory{};

	float getSimulationTime() const;
	void resetTime();
	State getRHS(const State& state) const;
	void updateInertia();
};
