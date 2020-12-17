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

		void modifyVertexData()
		{
			glDeleteVertexArrays(1, &vertex_data->vao);
			glDeleteBuffers(3, vertex_data->vbo);
			glDeleteBuffers(1, &vertex_data->ebo);
			free(vertex_data);
			vertex_data = nullptr;
		}
};