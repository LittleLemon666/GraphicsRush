#pragma once
#include "Utilities/Pnt3f.H"
#include "Utilities/3DUtils.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

/*
 * from OpenGL-tutorial
 * url: https://github.com/opengl-tutorials/ogl/blob/master/common/objloader.cpp
 */

struct PackedVertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

class Model
{
public:
	Model();
	Model(const std::string, bool uv_reflect = false);
	Model(const Model& model);

	Model operator=(const Model& model);

	bool loadOBJ(const std::string fileName);

	void draw();

	~Model();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	glm::vec3 pos;

private:
	GLuint VertexArrayID;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	void VBOIndex(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals, bool uv_reflect);
	void VAOProcess();
	bool getSimilarVertexIndex_fast(PackedVertex& packed, std::map<PackedVertex, unsigned short>& VertexToOutIndex, unsigned short& result);
};