#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GL/glu.h>
//************************************************************************
//
// * capture Pixelation by 360 camera
//
//========================================================================
class Shadow
{
public:
	static const unsigned int SHADOW_WIDTH = 1024;
	static const unsigned int SHADOW_HEIGHT = 1024;

	Shadow() : size(1024, 1024)
	{
		GeneratingDepthMap();
	}

	~Shadow() {}
	const GLuint getDepthMapFBO() const { return depthMapFBO; }
	const GLuint getID() const { return id; }
	const glm::vec2 getSize() const { return size; }

	const void bind(GLenum bind_unit)
	{
		glActiveTexture(GL_TEXTURE0 + bind_unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}

private:
	GLuint id;
	GLuint depthMapFBO;

	void GeneratingDepthMap()
	{
		//************************************************************************
		//
		// * generate a new texture
		//
		//========================================================================
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glm::vec2 size;
};