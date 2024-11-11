#pragma once

#include "scene.hpp"
#include "simulation.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <optional>
#include <string>

class ControlPanel
{
public:
	ControlPanel(Scene& scene, Simulation& simulation, const glm::vec2& pos, const glm::vec2& size);
	void update();

private:
	Scene& m_scene;
	Simulation& m_simulation;
	const glm::vec2 m_pos;
	const glm::vec2 m_size;

	void updateFloatValue(const std::function<float()>& get, const std::function<void(float)>& set,
		const std::string& name, std::optional<float> min = std::nullopt,
		const std::string& format = "%.1f", float step = 0.1f);
	void updateIntValue(const std::function<int()>& get, const std::function<void(int)>& set,
		const std::string& name, std::optional<int> min = std::nullopt);
	void updateCheckbox(const std::function<bool()>& get, const std::function<void(bool)>& set,
		const std::string& name);
	void separator();
};
