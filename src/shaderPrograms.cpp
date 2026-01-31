#include "shaderPrograms.hpp"

#include <string>

namespace ShaderPrograms
{
	std::string path(const std::string& shaderName);

	std::unique_ptr<const ShaderProgram> mesh{};
	std::unique_ptr<const ShaderProgram> polyline{};
	std::unique_ptr<const ShaderProgram> plane{};

	void init()
	{
		mesh = std::make_unique<const ShaderProgram>(path("meshVS"), path("meshFS"));
		polyline = std::make_unique<const ShaderProgram>(path("polylineVS"), path("polylineFS"));
		plane = std::make_unique<const ShaderProgram>(path("planeVS"), path("planeFS"));
	}

	std::string path(const std::string& shaderName)
	{
		return "src/shaders/" + shaderName + ".glsl";
	}
}
