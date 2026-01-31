#pragma once

#include "shaderProgram.hpp"

#include <memory>

namespace ShaderPrograms
{
	void init();

	extern std::unique_ptr<const ShaderProgram> mesh;
	extern std::unique_ptr<const ShaderProgram> polyline;
	extern std::unique_ptr<const ShaderProgram> plane;
}
