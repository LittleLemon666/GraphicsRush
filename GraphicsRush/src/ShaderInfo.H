#pragma once
#include <vector>
#include <GL/glu.h>
#include "RenderUtilities/BufferObject.h"

class ShaderInfo {
	public:
		std::vector<GLfloat> vertices = {};
		std::vector<GLfloat> normal = {};
		std::vector<GLfloat> texture_coordinate = {};
		std::vector<GLuint> element = {};

		VAO* vertex_data = nullptr;
};