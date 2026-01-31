#include "simulation.hpp"

#include "rungeKutta.hpp"

#include <cmath>

Simulation::Simulation()
{
	updateInertia();
}

void Simulation::update()
{
	if (!m_running)
	{
		return;
	}

	float frameT = getSimulationTime();
	int iterations = static_cast<int>(frameT / m_dT);

	while (m_t.size() <= iterations)
	{
		float prevT = (m_t.size() - 1) * m_dT;
		float t = prevT + m_dT;
		m_state = State{RungeKutta::RK4(prevT, m_dT, m_state.toArray(),
			[this] (float, const RungeKutta::State& state)
			{
				return getRHS(state).toArray();
			}
		)};
		m_state.normalize();

		m_t.push_back(t);
		m_trajectory.push_back(m_state.orientation * glm::vec3{0, std::sqrt(3.0f) * m_side, 0});
	}
}

void Simulation::stop()
{
	m_running = false;
}

void Simulation::start()
{
	if (m_running)
	{
		return;
	}

	glm::mat3 orientationMatrix =
	{
		std::cos(m_angle0), std::sin(m_angle0), 0,
		-std::sin(m_angle0), std::cos(m_angle0), 0,
		0, 0, 1
	};

	m_state.orientation = glm::quat_cast(orientationMatrix);
	m_state.angVelocity = {0, m_angVelocity0, 0};

	m_t.clear();
	m_trajectory.clear();

	m_t.push_back(0);
	m_trajectory.push_back(m_state.orientation * glm::vec3{0, std::sqrt(3) * m_side, 0});

	resetTime();
	m_running = true;
}

glm::mat3 Simulation::initialRotation()
{
	static constexpr float piOver4 = glm::pi<float>() * 0.25f;

	static const glm::mat3 Ry =
	{
		std::cos(piOver4), 0, -std::sin(piOver4),
		0, 1.0f, 0,
		std::sin(piOver4), 0, std::cos(piOver4)
	};

	static const float angle = std::atan(std::sqrt(2.0f));
	static const glm::mat3 Rz =
	{
		std::cos(angle), std::sin(angle), 0,
		-std::sin(angle), std::cos(angle), 0,
		0, 0, 1.0f
	};

	static const glm::mat3 rotation = Rz * Ry;

	return rotation;
}

float Simulation::getDT() const
{
	return m_dT;
}

void Simulation::setDT(float dT)
{
	if (m_running)
	{
		return;
	}

	m_dT = dT;
}

float Simulation::getDensity() const
{
	return m_density;
}

void Simulation::setDensity(float density)
{
	m_density = density;
	updateInertia();
}

float Simulation::getSide() const
{
	return m_side;
}

void Simulation::setSide(float side)
{
	m_side = side;
	updateInertia();
}

float Simulation::getAngle0() const
{
	return m_angle0;
}

void Simulation::setAngle0(float angle0)
{
	if (m_running)
	{
		return;
	}

	m_angle0 = angle0;
}

float Simulation::getAngVelocity0() const
{
	return m_angVelocity0;
}

void Simulation::setAngVelocity0(float angVelocity0)
{
	if (m_running)
	{
		return;
	}

	m_angVelocity0 = angVelocity0;
}

std::size_t Simulation::getTrajectoryLength() const
{
	return m_trajectoryLength;
}

void Simulation::setTrajectoryLength(std::size_t trajectoryLength)
{
	m_trajectoryLength = trajectoryLength;
}

bool Simulation::getGravity() const
{
	return m_gravity;
}

void Simulation::setGravity(bool gravity)
{
	m_gravity = gravity;
}

int Simulation::getIterations() const
{
	return static_cast<int>(m_t.size());
}

float Simulation::getT() const
{
	if (m_t.empty())
	{
		return 0;
	}
	return m_t.back();
}

glm::quat Simulation::getOrientation() const
{
	return m_state.orientation;
}

const std::vector<glm::vec3> Simulation::getTrajectory() const
{
	if (m_trajectory.size() <= m_trajectoryLength)
	{
		return m_trajectory;
	}

	return {m_trajectory.end() - m_trajectoryLength, m_trajectory.end()};
}

float Simulation::getSimulationTime() const
{
	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
	std::chrono::duration<float> simulationT = t - m_t0;
	return simulationT.count();
}

void Simulation::resetTime()
{
	m_t0 = std::chrono::system_clock::now();
}

State Simulation::getRHS(const State& state) const
{
	State stateDerivative{};

	glm::vec3 torque = {0, 0, 0};
	if (m_gravity)
	{
		static constexpr float g = 9.81f;

		glm::vec3 gravityGlobal{0, -m_mass * g, 0};
		glm::vec3 gravity = glm::conjugate(state.orientation) * gravityGlobal;
		torque = glm::cross(glm::vec3{0, m_side * std::sqrt(3.0f) * 0.5f, 0}, gravity);
	}

	stateDerivative.orientation = state.orientation * glm::quat{0, state.angVelocity} * 0.5f;

	stateDerivative.angVelocity = m_inertiaTensorInverse * (torque +
		glm::cross(m_inertiaTensor * state.angVelocity, state.angVelocity));

	return stateDerivative;
}

void Simulation::updateInertia()
{
	m_mass = m_density * std::pow(m_side, 3.0f);

	static constexpr glm::mat3 unitCubeInertiaTensor =
	{
		2.0f / 3.0f, -1.0f / 4.0f, -1.0f / 4.0f,
		-1.0f / 4.0f, 2.0f / 3.0f, -1.0f / 4.0f,
		-1.0f / 4.0f, -1.0f / 4.0f, 2.0f / 3.0f
	};

	glm::mat3 inertiaTensor = m_mass * std::pow(m_side, 3.0f) * unitCubeInertiaTensor;

	m_inertiaTensor = initialRotation() * inertiaTensor * glm::transpose(initialRotation());
	m_inertiaTensorInverse = glm::inverse(m_inertiaTensor);
}
