#include "guis/controlPanel.hpp"

#include <imgui/imgui.h>

#include <algorithm>

ControlPanel::ControlPanel(Scene& scene, Simulation& simulation, const glm::vec2& pos,
	const glm::vec2& size) :
	m_scene{scene},
	m_simulation{simulation},
	m_pos{pos},
	m_size{size}
{ }

void ControlPanel::update()
{
	ImGui::SetNextWindowPos({m_pos.x, m_pos.y}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({m_size.x, m_size.y}, ImGuiCond_Always);
	ImGui::Begin("controlPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	if (ImGui::Button("Start"))
	{
		m_simulation.start();
	}

	ImGui::Spacing();

	if (ImGui::Button("Stop"))
	{
		m_simulation.stop();
	}

	ImGui::Spacing();

	ImGui::Text("t = %.2f", m_simulation.getT());

	separator();

	updateFloatValue
	(
		[this] () { return m_simulation.getDT(); },
		[this] (float dt) { m_simulation.setDT(dt); },
		"dt",
		0.001f,
		"%.3f",
		0.001f
	);

	updateFloatValue
	(
		[this] () { return m_simulation.getDensity(); },
		[this] (float density) { m_simulation.setDensity(density); },
		"density",
		0.1f
	);

	updateFloatValue
	(
		[this] () { return m_simulation.getSide(); },
		[this] (float side) { m_simulation.setSide(side); },
		"side",
		0.1f
	);

	updateFloatValue
	(
		[this] () { return glm::degrees(m_simulation.getAngle0()); },
		[this] (float angle0) { m_simulation.setAngle0(glm::radians(angle0)); },
		"angle",
		0.0f
	);

	updateFloatValue
	(
		[this] () { return m_simulation.getAngVelocity0(); },
		[this] (float angVelocity0) { m_simulation.setAngVelocity0(angVelocity0); },
		"angular velocity",
		0.0f,
		"%.0f"
	);

	updateIntValue
	(
		[this] () { return static_cast<int>(m_simulation.getTrajectoryLength()); },
		[this] (int trajectoryLength) { m_simulation.setTrajectoryLength(trajectoryLength); },
		"trajectoryLength",
		0
	);

	separator();

	updateCheckbox
	(
		[this] () { return m_simulation.getGravity(); },
		[this] (bool gravity) { m_simulation.setGravity(gravity); },
		"gravity"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderCube(); },
		[this] (bool renderCube) { m_scene.setRenderCube(renderCube); },
		"render cube"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderDiagonal(); },
		[this] (bool renderDiagonal) { m_scene.setRenderDiagonal(renderDiagonal); },
		"render diagonal"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderTrajectory(); },
		[this] (bool renderTrajectory) { m_scene.setRenderTrajectory(renderTrajectory); },
		"render trajectory"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderGravityVector(); },
		[this] (bool renderGravityVector) { m_scene.setRenderGravityVector(renderGravityVector); },
		"render gravity vector"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderPlane(); },
		[this] (bool renderPlane) { m_scene.setRenderPlane(renderPlane); },
		"render plane"
	);

	ImGui::End();
}

void ControlPanel::updateFloatValue(const std::function<float()>& get,
	const std::function<void(float)>& set, const std::string& name, std::optional<float> min,
	const std::string& format, float step)
{
	static const std::string suffix = "##controlPanelFloatValue";

	ImGui::PushItemWidth(100);

	float value = get();
	float prevValue = value;
	ImGui::InputFloat((name + suffix).c_str(), &value, step, step, format.c_str());
	if (min.has_value())
	{
		value = std::max(value, *min);
	}
	if (value != prevValue)
	{
		set(value);
	}

	ImGui::PopItemWidth();
}

void ControlPanel::updateIntValue(const std::function<int()>& get,
	const std::function<void(int)>& set, const std::string& name, std::optional<int> min)
{
	static const std::string suffix = "##controlPanelIntValue";
	static constexpr int step = 100;

	ImGui::PushItemWidth(100);

	int value = get();
	int prevValue = value;
	ImGui::InputInt((name + suffix).c_str(), &value, step, step);
	if (min.has_value())
	{
		value = std::max(value, *min);
	}
	if (value != prevValue)
	{
		set(value);
	}

	ImGui::PopItemWidth();
}

void ControlPanel::updateCheckbox(const std::function<bool()>& get,
	const std::function<void(bool)>& set, const std::string& name)
{
	static const std::string suffix = "##controlPanelCheckbox";

	bool value = get();
	bool prevValue = value;
	ImGui::Checkbox((name + suffix).c_str(), &value);
	if (value != prevValue)
	{
		set(value);
	}
}

void ControlPanel::separator()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}
