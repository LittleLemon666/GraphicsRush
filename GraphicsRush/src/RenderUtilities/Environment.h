#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GL/glu.h>
//************************************************************************
//
// * capture environment by 360 camera
//
//========================================================================
class Environment
{
public:
	Environment() : size(256, 256)
	{
		GeneratingDynamicCubemap(256, 256, glm::vec3(0, 0, 0));
	}

	Environment(int w, int h, glm::vec3 _center) : center(_center), size(w, h)
	{
		GeneratingDynamicCubemap(w, h, _center);
	}

	~Environment() {}
	const GLuint getID() const { return id; }
	const glm::vec2 getSize() const { return size; }
	const glm::vec3 getCameraPos() const { return center; }
	const void setCameraPos(glm::vec3 _center) { center = _center; }

	const void switchToFace(int faceIndex)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		switch (faceIndex)
		{
		case 0: // +X (right)
			gluLookAt(0, 0, 0, 1, 0, 0, 0, -1, 0);
			break;
		case 1: // -X (left)
			gluLookAt(0, 0, 0, -1, 0, 0, 0, -1, 0);
			break;
		case 2: // +Y (top)
			gluLookAt(0, 0, 0, 0, 1, 0, 0, 0, 1);
			break;
		case 3: // -Y (bottom)
			gluLookAt(0, 0, 0, 0, -1, 0, 0, 0, 1);
			break;
		case 4: // -Z (back)
			gluLookAt(0, 0, 0, 0, 0, 1, 0, -1, 0);
			break;
		case 5: // +Z (front)
			gluLookAt(0, 0, 0, 0, 0, -1, 0, -1, 0);
			break;
		}
		glTranslatef(-center.x, -center.y, -center.z);
	}

	const void bind(GLenum bind_unit)
	{
		glActiveTexture(GL_TEXTURE0 + bind_unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	}

private:
	GLuint id;

	void GeneratingDynamicCubemap(int width, int height, glm::vec3 cameraCenter)
	{
		//************************************************************************
		//
		// * generate a new cubemap
		//
		//========================================================================
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		for (GLuint i = 0; i < 6; i++) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	glm::vec2 size;
	glm::vec3 center = glm::vec3();
};