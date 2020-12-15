#pragma once
#include <vector>
#include <GL/glu.h>
class ShaderInfo {
	public:
		std::vector<GLfloat> vertices = {};
		std::vector<GLfloat> normal = {};
		std::vector<GLfloat> texture_coordinate = {};
		std::vector<GLuint> element = {};
};