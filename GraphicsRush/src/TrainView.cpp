/************************************************************************
	 File:        TrainView.cpp
	 Author:
				  Michael Gleicher, gleicher@cs.wisc.edu
	 Modifier
				  Yu-Chi Lai, yu-chi@cs.wisc.edu
	 Comment:
						The TrainView is the window that actually shows the
						train. Its a
						GL display canvas (Fl_Gl_Window).  It is held within
						a TrainWindow
						that is the outer window with all the widgets.
						The TrainView needs
						to be aware of the window - since it might need to
						check the widgets to see how to draw
	  Note:        we need to have pointers to this, but maybe not know
						about it (beware circular references)
	 Platform:    Visio Studio.Net 2003/2005
*************************************************************************/

#include <iostream>
#include <Fl/fl.h>

// we will need OpenGL, and OpenGL needs windows.h
#include <windows.h>
//#include "GL/gl.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glu.h>

#include "TrainView.H"
#include "TrainWindow.H"
#include "Utilities/3DUtils.H"

/*********************NEW ADDITIONS*********************/
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace glm;

#ifdef EXAMPLE_SOLUTION
#	include "TrainExample/TrainExample.H"
#endif


//************************************************************************
//
// * Constructor to set up the GL window
//========================================================================
TrainView::
TrainView(int x, int y, int w, int h, const char* l)
	: Fl_Gl_Window(x, y, w, h, l)
	//========================================================================
{
	mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);

	resetArcball();

	/*********************NEW ADDITIONS*********************/
	srand(time(NULL));

	// loads a cubemap texture from 6 individual texture faces
	// order:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// -Z (back)
	// +Z (front) 
	// -------------------------------------------------------
	chapters_skybox_textures_faces.push_back(std::vector<std::string>());
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/px.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/nx.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/py.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/ny.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/nz.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/pz.png");

	chapters_skybox_textures_faces.push_back(std::vector<std::string>());
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/maze/right.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/maze/left.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/maze/top.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/maze/bottom.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/maze/back.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/maze/front.jpg");

	chapters_skybox_textures_faces.push_back(std::vector<std::string>());
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/rollercoaster/right.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/rollercoaster/left.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/rollercoaster/top.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/rollercoaster/bottom.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/rollercoaster/back.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/rollercoaster/front.png");

	chapters_skybox_textures_faces.push_back(std::vector<std::string>());
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/skybox/right.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/skybox/left.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/skybox/top.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/skybox/bottom.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/skybox/back.jpg");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/skybox/front.jpg");

	chapters_skybox_textures_faces.push_back(std::vector<std::string>());
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/px.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/nx.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/py.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/ny.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/nz.png");
	chapters_skybox_textures_faces.back().push_back("../GraphicsRush/Images/classroom/pz.png");

	chapter_path_file.push_back("../GraphicsRush/TrackFiles/P1.txt");
	chapter_path_file.push_back("../GraphicsRush/TrackFiles/P2.txt");
	chapter_path_file.push_back("../GraphicsRush/TrackFiles/P3.txt");
	chapter_path_file.push_back("../GraphicsRush/TrackFiles/P4.txt");
	chapter_path_file.push_back("../GraphicsRush/TrackFiles/P5.txt");
	for (auto it = chapter_path_file.begin(); it != chapter_path_file.end(); it++)
		chapter_path_file_name.push_back(getFileName(*it));
}

//************************************************************************
//
// * Reset the camera to look at the world
//========================================================================
void TrainView::
resetArcball()
//========================================================================
{
	// Set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this, 40, 250, .2f, .4f, 0);
}

//************************************************************************
//
// * FlTk Event handler for the window
//########################################################################
// TODO: 
//       if you want to make the train respond to other events 
//       (like key presses), you might want to hack this.
//########################################################################
//========================================================================
int TrainView::
handle(int event)
{
	// see if the ArcBall will handle the event - if it does, 
	// then we're done
	// note: the arcball only gets the event if we're in world view
	if (tw->worldCam->value())
		if (arcball.handle(event))
			return 1;

	// remember what button was used
	static int last_push;

	if (!tw->trainCam->value())
	{
		switch (event) {
			// Mouse button being pushed event
		case FL_PUSH:
			last_push = Fl::event_button();
			// if the left button be pushed is left mouse button
			if (last_push == FL_LEFT_MOUSE) {
				doPick();
				damage(1);
				return 1;
			};
			break;

			// Mouse button release event
		case FL_RELEASE: // button release
			damage(1);
			last_push = 0;
			return 1;

			// Mouse button drag event
		case FL_DRAG:

			// Compute the new control point position
			if ((last_push == FL_LEFT_MOUSE) && (selectedCube >= 0)) {
				ControlPoint* cp = &m_pTrack->points[selectedCube];

				double r1x, r1y, r1z, r2x, r2y, r2z;
				getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);

				double rx, ry, rz;
				mousePoleGo(r1x, r1y, r1z, r2x, r2y, r2z,
					static_cast<double>(cp->pos.x),
					static_cast<double>(cp->pos.y),
					static_cast<double>(cp->pos.z),
					rx, ry, rz,
					(Fl::event_state() & FL_CTRL) != 0);

				cp->pos.x = (float)rx;
				cp->pos.y = (float)ry;
				cp->pos.z = (float)rz;
				damage(1);
			}
			break;

			// in order to get keyboard events, we need to accept focus
		case FL_FOCUS:
			return 1;

			// every time the mouse enters this window, aggressively take focus
		case FL_ENTER:
			focus(this);
			break;

		case FL_KEYBOARD:
			int k = Fl::event_key();
			int ks = Fl::event_state();
			if (k == 'p') {
				// Print out the selected control point information
				if (selectedCube >= 0)
					printf("Selected(%d) (%g %g %g) (%g %g %g)\n",
						selectedCube,
						m_pTrack->points[selectedCube].pos.x,
						m_pTrack->points[selectedCube].pos.y,
						m_pTrack->points[selectedCube].pos.z,
						m_pTrack->points[selectedCube].orient.x,
						m_pTrack->points[selectedCube].orient.y,
						m_pTrack->points[selectedCube].orient.z);
				else
					printf("Nothing Selected\n");

				return 1;
			};
			break;
		}
	}

	return Fl_Gl_Window::handle(event);
}

/*********************NEW ADDITIONS*********************/
void TrainView::
initPath() {
	path->vertices = {};
	path->normal = {};
	path->texture_coordinate = {};
	path->element = {};
	vec3 first_segment_r(-1.0f, -1.0f, -1.0f);
	vec3 first_segment_l(-1.0f, -1.0f, -1.0f);
	vec3 last_segment_r(-1.0f, -1.0f, -1.0f);
	vec3 last_segment_l(-1.0f, -1.0f, -1.0f);
	const int NUM_of_CPs = (int)m_pTrack->points.size();
	for (int cp_id = 0; cp_id < NUM_of_CPs; cp_id++) {

		//create forward vector
		vec3 this_cp = vec3(m_pTrack->points[cp_id].pos.x,
			m_pTrack->points[cp_id].pos.y,
			m_pTrack->points[cp_id].pos.z);
		int next_cp_id = (cp_id + 1) % NUM_of_CPs;
		vec3 next_cp = vec3(m_pTrack->points[next_cp_id].pos.x,
			m_pTrack->points[next_cp_id].pos.y,
			m_pTrack->points[next_cp_id].pos.z);
		vec3 forward = next_cp - this_cp;

		for (int segment = 0; segment < PATH_DIVIDE; segment++) {
			//create forward vector for this segment
			vec3 this_segment = this_cp + forward * ((float)segment / (float)PATH_DIVIDE);
			vec3 next_segment = this_cp + forward * ((float)(segment + 1) / (float)PATH_DIVIDE);

			//overide linear track with BSpline track
			gmt.setG_pos(cp_id);
			this_segment = gmt.calculate((float)segment / (float)PATH_DIVIDE);
			next_segment = gmt.calculate((float)(segment + 1) / (float)PATH_DIVIDE);

			vec3 segment_forward = next_segment - this_segment;
			segment_forward = normalize(segment_forward);

			//create orient vector
			gmt.setG_orient(cp_id);
			vec3 this_cp_orient = vec3(
				m_pTrack->points[cp_id].orient.x * (1.0f - (float)segment / (float)PATH_DIVIDE) + m_pTrack->points[next_cp_id].orient.x * ((float)segment / (float)PATH_DIVIDE),
				m_pTrack->points[cp_id].orient.y * (1.0f - (float)segment / (float)PATH_DIVIDE) + m_pTrack->points[next_cp_id].orient.y * ((float)segment / (float)PATH_DIVIDE),
				m_pTrack->points[cp_id].orient.z * (1.0f - (float)segment / (float)PATH_DIVIDE) + m_pTrack->points[next_cp_id].orient.z * ((float)segment / (float)PATH_DIVIDE));
			this_cp_orient = gmt.calculate((float)segment / (float)PATH_DIVIDE);
			this_cp_orient = normalize(this_cp_orient);
			vec3 next_cp_orient = vec3(
				m_pTrack->points[cp_id].orient.x * (1.0f - (float)(segment + 1) / (float)PATH_DIVIDE) + m_pTrack->points[next_cp_id].orient.x * ((float)(segment + 1) / (float)PATH_DIVIDE),
				m_pTrack->points[cp_id].orient.y * (1.0f - (float)(segment + 1) / (float)PATH_DIVIDE) + m_pTrack->points[next_cp_id].orient.y * ((float)(segment + 1) / (float)PATH_DIVIDE),
				m_pTrack->points[cp_id].orient.z * (1.0f - (float)(segment + 1) / (float)PATH_DIVIDE) + m_pTrack->points[next_cp_id].orient.z * ((float)(segment + 1) / (float)PATH_DIVIDE));
			next_cp_orient = gmt.calculate((float)(segment + 1) / (float)PATH_DIVIDE);
			next_cp_orient = normalize(next_cp_orient);

			//create cross vector
			vec3 this_cross = cross(segment_forward, this_cp_orient);
			this_cross = normalize(this_cross);
			vec3 next_cross = cross(segment_forward, next_cp_orient);
			next_cross = normalize(next_cross);
			//initialize path->vertices
			GLfloat roadSize = 10.0f;
			this_cross = roadSize * this_cross;
			next_cross = roadSize * next_cross;
			//record next segment and the first segment so later it can be used to perfectly connect segments
			if (cp_id == 0 && segment == 0) {
				first_segment_r = vec3(
					this_segment.x + this_cross.x,
					this_segment.y + this_cross.y,
					this_segment.z + this_cross.z);
				first_segment_l = vec3(
					this_segment.x - this_cross.x,
					this_segment.y - this_cross.y,
					this_segment.z - this_cross.z);
				path->vertices.push_back(this_segment.x - this_cross.x);
				path->vertices.push_back(this_segment.y - this_cross.y);
				path->vertices.push_back(this_segment.z - this_cross.z);
				path->vertices.push_back(this_segment.x + this_cross.x);
				path->vertices.push_back(this_segment.y + this_cross.y);
				path->vertices.push_back(this_segment.z + this_cross.z);
			}
			else {
				path->vertices.push_back(last_segment_l.x);
				path->vertices.push_back(last_segment_l.y);
				path->vertices.push_back(last_segment_l.z);
				path->vertices.push_back(last_segment_r.x);
				path->vertices.push_back(last_segment_r.y);
				path->vertices.push_back(last_segment_r.z);
			}
			if (cp_id + 1 == NUM_of_CPs && segment + 1 == PATH_DIVIDE) {
				path->vertices.push_back(first_segment_r.x);
				path->vertices.push_back(first_segment_r.y);
				path->vertices.push_back(first_segment_r.z);
				path->vertices.push_back(first_segment_l.x);
				path->vertices.push_back(first_segment_l.y);
				path->vertices.push_back(first_segment_l.z);
			}
			else {
				path->vertices.push_back(next_segment.x + next_cross.x);
				path->vertices.push_back(next_segment.y + next_cross.y);
				path->vertices.push_back(next_segment.z + next_cross.z);
				path->vertices.push_back(next_segment.x - next_cross.x);
				path->vertices.push_back(next_segment.y - next_cross.y);
				path->vertices.push_back(next_segment.z - next_cross.z);
			}
			last_segment_r = vec3(
				next_segment.x + next_cross.x,
				next_segment.y + next_cross.y,
				next_segment.z + next_cross.z);
			last_segment_l = vec3(
				next_segment.x - next_cross.x,
				next_segment.y - next_cross.y,
				next_segment.z - next_cross.z);

			//initialize path->normal
			path->normal.push_back(this_cp_orient.x);
			path->normal.push_back(this_cp_orient.y);
			path->normal.push_back(this_cp_orient.z);
			path->normal.push_back(this_cp_orient.x);
			path->normal.push_back(this_cp_orient.y);
			path->normal.push_back(this_cp_orient.z);
			path->normal.push_back(next_cp_orient.x);
			path->normal.push_back(next_cp_orient.y);
			path->normal.push_back(next_cp_orient.z);
			path->normal.push_back(next_cp_orient.x);
			path->normal.push_back(next_cp_orient.y);
			path->normal.push_back(next_cp_orient.z);

			//initialize path->texture_coordinate
			path->texture_coordinate.push_back((GLfloat)0.0);
			path->texture_coordinate.push_back((GLfloat)0.0);
			path->texture_coordinate.push_back((GLfloat)1.0);
			path->texture_coordinate.push_back((GLfloat)0.0);
			path->texture_coordinate.push_back((GLfloat)1.0);
			path->texture_coordinate.push_back((GLfloat)1.0);
			path->texture_coordinate.push_back((GLfloat)0.0);
			path->texture_coordinate.push_back((GLfloat)1.0);

			//initialize path->element
			path->element.push_back(cp_id * PATH_DIVIDE * 4 + segment * 4);
			path->element.push_back(cp_id * PATH_DIVIDE * 4 + segment * 4 + 1);
			path->element.push_back(cp_id * PATH_DIVIDE * 4 + segment * 4 + 2);
			path->element.push_back(cp_id * PATH_DIVIDE * 4 + segment * 4);
			path->element.push_back(cp_id * PATH_DIVIDE * 4 + segment * 4 + 2);
			path->element.push_back(cp_id * PATH_DIVIDE * 4 + segment * 4 + 3);
		}
	}

	if (!this->path->vertex_data)
	{
		this->path->vertex_data = new VAO;
		glGenVertexArrays(1, &this->path->vertex_data->vao);
		glGenBuffers(3, this->path->vertex_data->vbo);
		glGenBuffers(1, &this->path->vertex_data->ebo);
	}
	else 
	{
		glad_glDeleteVertexArrays(1, &this->path->vertex_data->vao);
		glad_glDeleteBuffers(3, this->path->vertex_data->vbo);
		glad_glDeleteBuffers(1, &this->path->vertex_data->ebo);
		glGenVertexArrays(1, &this->path->vertex_data->vao);
		glGenBuffers(3, this->path->vertex_data->vbo);
		glGenBuffers(1, &this->path->vertex_data->ebo);
	}
	this->path->vertex_data->element_amount = (int)path->element.size() * sizeof(GLuint);	

	glBindVertexArray(this->path->vertex_data->vao);

	// Position attribute
	glBindBuffer(GL_ARRAY_BUFFER, this->path->vertex_data->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, (int)path->vertices.size() * sizeof(GLfloat), &path->vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glBindBuffer(GL_ARRAY_BUFFER, this->path->vertex_data->vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, (int)path->normal.size() * sizeof(GLfloat), &path->normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	// Texture Coordinate attribute
	glBindBuffer(GL_ARRAY_BUFFER, this->path->vertex_data->vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, (int)path->texture_coordinate.size() * sizeof(GLfloat), &path->texture_coordinate[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	//Element attribute
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->path->vertex_data->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)path->element.size() * sizeof(GLuint), &path->element[0], GL_STATIC_DRAW);

	// Unbind VAO
	glBindVertexArray(0);
};

GLuint TrainView::
loadCubemap(std::vector<std::string> chapters_skybox_textures_faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
	unsigned char* image;
	for (GLuint i = 0; i < chapters_skybox_textures_faces.size(); i++)
	{
		image = stbi_load(chapters_skybox_textures_faces[i].c_str(), &width, &height, 0, 0);
		if (image)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			stbi_image_free(image);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << chapters_skybox_textures_faces[i] << std::endl;
			stbi_image_free(image);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void TrainView::
switchChapter(const int& chapter_index)
{
	chapter = chapter_index;
	if (chapter >= chapter_path_file.size()) chapter = rand() % chapter_path_file.size();
	load_chapter = false;
}

void TrainView::
initSkybox()
{
	if (!this->skybox->vertex_data)
	{
		GLfloat vertices[] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		this->skybox->vertex_data = new VAO;
		glGenVertexArrays(1, &this->skybox->vertex_data->vao);
		glGenBuffers(1, this->skybox->vertex_data->vbo);

		glBindVertexArray(this->skybox->vertex_data->vao);

		glBindBuffer(GL_ARRAY_BUFFER, this->skybox->vertex_data->vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Unbind VAO
		glBindVertexArray(0);
	}
}

void TrainView::
initDoor()
{
	if (!this->door)
	{
		this->door = new ShaderInfo;
		GLfloat vertices[] = {
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f
		};

		GLfloat texture_coordinate[] = {
				0.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 1.0f,
				1.0f, 0.0f
		};

		this->door->vertex_data = new VAO;
		glGenVertexArrays(1, &this->door->vertex_data->vao);
		glGenBuffers(2, this->door->vertex_data->vbo);

		glBindVertexArray(this->door->vertex_data->vao);

		glBindBuffer(GL_ARRAY_BUFFER, this->door->vertex_data->vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, this->door->vertex_data->vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coordinate), &texture_coordinate, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);

		// Unbind VAO
		glBindVertexArray(0);
	}
}

bool TrainView::
initText()
{
	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		puts("ERROR::FREETYPE: Could not init FreeType Library");
		return false;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		puts("ERROR::FREETYPE: Failed to load font");
		return false;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				puts("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x * 1.4f)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// configure VAO/VBO for texture quads
	// -----------------------------------
	font_vertex_data = new VAO;
	glGenVertexArrays(1, &font_vertex_data->vao);
	glGenBuffers(1, font_vertex_data->vbo);
	glBindVertexArray(font_vertex_data->vao);
	glBindBuffer(GL_ARRAY_BUFFER, font_vertex_data->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void TrainView::
initCameraMovement()
{
	camera_movement = {};
	camera_movement.push_back(std::vector<vec3>());

	//prepare variables
	vec3 orient, crossed, thisPosition, nextPosition;
	//find trainPosition, forward and orient
	gmt.setG_pos(0);
	train_pos = gmt.calculate(0);

	nextPosition = gmt.calculate(1.0f / PATH_DIVIDE);
	forward = vec3(nextPosition - train_pos);

	gmt.setG_orient(0);
	thisPosition = gmt.calculate(0);
	nextPosition = gmt.calculate(1.0f / PATH_DIVIDE);
	orient = thisPosition;

	//find playerUp (the orient perpendicular to the rail)
	crossed = cross(forward, orient);
	up = cross(crossed, forward);

	//normalize all vec3s for use
	forward = normalize(forward);
	orient = normalize(orient);
	up = normalize(up);
	crossed = normalize(crossed);
	forward = -normalize(forward + up * 1.5f);
	vec3 viewer_forward = forward;
	vec3 viewer_up = up;
	vec3 viewer_pos = train_pos - viewer_forward * 20.0f + viewer_up;
	//set look at (trainPosition(viewerPosition) -> where to look at -> up)
	for (int i = 0; i < 25; i++)
	{
		camera_movement[0].push_back(viewer_pos);
		camera_movement[0].push_back(viewer_forward);
		camera_movement[0].push_back(viewer_up);
	}
	for (float camera_angle = 0, dt = 0; camera_angle < 150; camera_angle += 10.0f, dt += 2.0f / PATH_DIVIDE)
	{
		viewer_forward = normalize(vec3(forward.x * cos(radians(camera_angle)) + forward.z * sin(radians(camera_angle)),
			forward.y,
			forward.x * sin(radians(camera_angle)) + forward.z * cos(radians(camera_angle))));
		
		gmt.setG_pos(dt);

		viewer_pos = gmt.calculate(dt) - viewer_forward * (20.0f - camera_angle / 10.0f);
		//viewer_pos.y -= camera_angle / 10.0f;
		//vec3 viewer_up = normalize(cross(viewer_forward, crossed));
		viewer_up = vec3(up.x * cos(radians(camera_angle)) + up.z * sin(radians(camera_angle)),
			up.y,
			up.x * sin(radians(camera_angle)) + up.z * cos(radians(camera_angle)));

		viewer_pos += viewer_up * (10.0f * camera_angle / 180.0f);
		camera_movement[0].push_back(viewer_pos);
		camera_movement[0].push_back(viewer_forward);
		camera_movement[0].push_back(viewer_up);
	}
}

void TrainView::
cameraMovement()
{
	if (camera_movement_state == 0)
	{
		vec3 viewer_pos = camera_movement[camera_movement_state][camera_movement_index++];
		forward = camera_movement[camera_movement_state][camera_movement_index++];
		up = camera_movement[camera_movement_state][camera_movement_index++];
		//vec3 viewer_pos = train_pos + up * 20.0f - forward * 10.0f;

		gluLookAt(viewer_pos.x, viewer_pos.y, viewer_pos.z,
			viewer_pos.x + forward.x * 10.0f,
			viewer_pos.y + forward.y * 10.0f,
			viewer_pos.z + forward.z * 10.0f,
			up.x, up.y, up.z);

		if (camera_movement_index >= camera_movement[camera_movement_state].size())
		{
			camera_movement_state++;
		}
	}
}

void TrainView::
initScreenRender()
{
	//************************************************************************
	//
	// * generate a new texture
	//
	//========================================================================
	if (load_screen)
		glad_glDeleteTextures(1, &screen_id);
	load_screen = true;
	glGenTextures(1, &screen_id);
	glBindTexture(GL_TEXTURE_2D, screen_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w(), h(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void TrainView::
renderScreenBegin()
{
	if (!screen_FBO)
	{
		screen_FBO = new FBO;
	}
	glGenFramebuffers(1, &screen_FBO->fbo);
	glGenRenderbuffers(1, &screen_FBO->rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, screen_FBO->fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBindRenderbuffer(GL_RENDERBUFFER, screen_FBO->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w(), h()); //GL_DEPTH_COMPONENT24
	// attach it
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, screen_FBO->rbo);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, screen_id, 0);

	// clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TrainView::
renderScreenEnd()
{
	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glad_glDeleteRenderbuffers(1, &screen_FBO->rbo);
	glad_glDeleteFramebuffers(1, &screen_FBO->fbo);
}

void TrainView::
initScreenQuad()
{
	if (!this->screen_quad)
	{
		GLfloat vertices[] = {
			-1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f
		};

		this->screen_quad = new VAO;
		glGenVertexArrays(1, &this->screen_quad->vao);
		glGenBuffers(1, this->screen_quad->vbo);

		glBindVertexArray(this->screen_quad->vao);

		glBindBuffer(GL_ARRAY_BUFFER, this->screen_quad->vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Unbind VAO
		glBindVertexArray(0);
	}
}

void TrainView::
drawPath() {
	//bind shader
	this->basic_shader->Use();

	glm::mat4 model_matrix = glm::mat4();
	model_matrix = glm::translate(model_matrix, this->source_pos);
	glUniformMatrix4fv(glGetUniformLocation(this->basic_shader->Program, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glUniform3fv(glGetUniformLocation(this->basic_shader->Program, "u_color"), 1, &glm::vec3(0.0f, 1.0f, 0.0f)[0]);
	this->path_texture->bind(0);
	glUniform1i(glGetUniformLocation(this->basic_shader->Program, "u_texture"), 0);

	//bind VAO
	glBindVertexArray(this->path->vertex_data->vao);

	glDrawElements(GL_TRIANGLES, this->path->vertex_data->element_amount, GL_UNSIGNED_INT, 0);

	//unbind VAO
	glBindVertexArray(0);

	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
};

void TrainView::
drawPlayer() {
	//bind shader
	this->basic_shader->Use();

	vec3 player_xbias;
	gmt.calculateAll(m_pTrack->trainU, player_pos, player_forward, player_up, player_xbias);
	player_forward = normalize(player_forward);
	player_up = normalize(player_up);
	player_xbias = normalize(player_xbias);
	player_pos += (float)m_pTrack->switchLane * player_xbias * 5.0f;
	if (m_pTrack->jumpingState > 0)
		player_pos += m_pTrack->airbornePosition[m_pTrack->jumpingState - 1] * player_up * 10.0f; //m_pTrack->jumpingState is added once in setProjection

	mat4 model_matrix = inverse(lookAt(player_pos + 5.0f * player_up, player_pos + player_forward + 5.0f * player_up, player_up)); // the player is in a 5.0f height position
	glUniformMatrix4fv(glGetUniformLocation(this->basic_shader->Program, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glUniform3fv(glGetUniformLocation(this->basic_shader->Program, "u_color"), 1, &vec3(0.0f, 1.0f, 0.0f)[0]);
	this->player_texture->bind(1);
	glUniform1i(glGetUniformLocation(this->basic_shader->Program, "u_texture"), 1);

	player_obj->draw();

	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
};

void TrainView::loadObjects() {
	if (!load_game_objects)
	{
		Obstacle::addObstacleModel(obstacle_obj_path, obstacle_texture_path);
		Money::addMoneyModel(money_obj_path, money_texture_path);
		load_game_objects = true;
	}
	
	float money_buffer = (float)((int)m_pTrack->points.size() - 2) / (float)m_pTrack->num_of_money;
	int money_to_obstacle_ratio = m_pTrack->num_of_money / m_pTrack->num_of_obstacles, current_ratio = 0;
	//printf("%d\n", money_to_obstacle_ratio);
	for (float distance = 1.0f; distance < (float)((int)m_pTrack->points.size() - 1); distance += money_buffer) {

		//add money
		vector<Money> line = {
			Money(distance, -1, 0), Money(distance, 0, 0), Money(distance, 1, 0)
			, Money(distance, -1, 1), Money(distance, 0, 1), Money(distance, 1, 1) };
		int money_spot = rand() % 6;
		if ((int)m_pTrack->money.size() != 0) {
			while (abs(((money_spot % 3) - 1) - m_pTrack->money[(int)m_pTrack->money.size() - 1].lane)
				+ abs((money_spot / 3) - m_pTrack->money[(int)m_pTrack->money.size() - 1].height) > 1) money_spot = rand() % 6;
		}
		m_pTrack->money.push_back(line[money_spot]);

		//add obstacle
		if (current_ratio == money_to_obstacle_ratio) {
			vector<Obstacle> wall = {
					Obstacle(distance, -1, 0), Obstacle(distance, 0, 0), Obstacle(distance, 1, 0)
					, Obstacle(distance, -1, 1), Obstacle(distance, 0, 1), Obstacle(distance, 1, 1) };
			int blocked = 5;
			for (int space = 0; space < 6; space++) {
				if (blocked == 0) break;
				if (rand() % 2 == 1 && space != money_spot) {
					m_pTrack->obstacles.push_back(wall[space]);
					blocked--;
				}
			}
			if (blocked == 5) {
				int space = rand() % 6;
				while (space == money_spot) space = rand() % 6;
				m_pTrack->obstacles.push_back(wall[space]);
			}
			current_ratio = 0;
		}
		current_ratio++;
	}
};

void TrainView::
drawObstacles() {
	this->basic_shader->Use();
	for (int obstacle = 0; obstacle < (int)m_pTrack->obstacles.size(); obstacle++) {
		vec3 obstaclePosition(0.0f, 0.0f, 0.0f), obstacleForward(0.0f, 0.0f, 0.0f), obstacleUp(0.0f, 0.0f, 0.0f), obstacleCross(0.0f, 0.0f, 0.0f);
		gmt.calculateAll(m_pTrack->obstacles[obstacle].position, obstaclePosition, obstacleForward, obstacleUp, obstacleCross);
		obstacleForward = normalize(obstacleForward);
		obstacleUp = normalize(obstacleUp);
		obstacleCross = normalize(obstacleCross);
		obstaclePosition += obstacleUp * 7.0f;
		obstaclePosition += obstacleCross * (float)m_pTrack->obstacles[obstacle].lane * 5.0f;
		float forwardSize = 2.0f;
		float upSize = 2.0f;
		float heightSize = 10.0f;
		if (m_pTrack->obstacles[obstacle].height) obstaclePosition += obstacleUp * heightSize;

		mat4 model_matrix = inverse(lookAt(obstaclePosition, obstaclePosition + obstacleForward * forwardSize, obstacleUp * upSize)); // the player is in a 5.0f height position
		model_matrix = scale(model_matrix, vec3(4.5, 4.5, 4.5));
		glUniformMatrix4fv(glGetUniformLocation(this->basic_shader->Program, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
		glUniform3fv(glGetUniformLocation(this->basic_shader->Program, "u_color"), 1, &vec3(0.0f, 1.0f, 0.0f)[0]);
		(m_pTrack->obstacles[obstacle]).obstacle_texture[0].bind(0);
		glUniform1i(glGetUniformLocation(this->basic_shader->Program, "u_texture"), 0);

		m_pTrack->obstacles[obstacle].obstacle_obj[0]->draw();
		/*
		glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(obstaclePosition.x + obstacleCross.x + obstacleUp.x, obstaclePosition.y + obstacleCross.y + obstacleUp.y, obstaclePosition.z + obstacleCross.z + obstacleUp.z);
		glVertex3f(obstaclePosition.x + obstacleCross.x - obstacleUp.x, obstaclePosition.y + obstacleCross.y - obstacleUp.y, obstaclePosition.z + obstacleCross.z - obstacleUp.z);
		glVertex3f(obstaclePosition.x - obstacleCross.x - obstacleUp.x, obstaclePosition.y - obstacleCross.y - obstacleUp.y, obstaclePosition.z - obstacleCross.z - obstacleUp.z);
		glVertex3f(obstaclePosition.x - obstacleCross.x + obstacleUp.x, obstaclePosition.y - obstacleCross.y + obstacleUp.y, obstaclePosition.z - obstacleCross.z + obstacleUp.z);
		glEnd();
		*/
	}
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

void TrainView::drawMoney() {
	this->basic_shader->Use();
	for (int obstacle = 0; obstacle < (int)m_pTrack->money.size(); obstacle++) {
		vec3 obstaclePosition(0.0f, 0.0f, 0.0f), obstacleForward(0.0f, 0.0f, 0.0f), obstacleUp(0.0f, 0.0f, 0.0f), obstacleCross(0.0f, 0.0f, 0.0f);
		gmt.calculateAll(m_pTrack->money[obstacle].position, obstaclePosition, obstacleForward, obstacleUp, obstacleCross);
		obstacleForward = normalize(obstacleForward);
		obstacleUp = normalize(obstacleUp);
		obstacleCross = normalize(obstacleCross);
		obstaclePosition += obstacleUp * 7.0f;
		obstaclePosition += obstacleCross * (float)m_pTrack->money[obstacle].lane * 5.0f;
		float forwardSize = 1.0f;
		float upSize = 1.0f;
		float heightSize = 10.0f;
		if (m_pTrack->money[obstacle].height) obstaclePosition += obstacleUp * heightSize;
		
		mat4 model_matrix = inverse(lookAt(obstaclePosition, obstaclePosition + obstacleForward * forwardSize, obstacleUp * upSize)); // the player is in a 5.0f height position
		model_matrix = rotate(model_matrix, money_rotate, vec3(0, 1, 0));
		model_matrix = scale(model_matrix, vec3(4.5, 4.5, 4.5));
		glUniformMatrix4fv(glGetUniformLocation(this->basic_shader->Program, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
		glUniform3fv(glGetUniformLocation(this->basic_shader->Program, "u_color"), 1, &vec3(0.0f, 1.0f, 0.0f)[0]);
		m_pTrack->money[obstacle].money_texture[0].bind(0);
		glUniform1i(glGetUniformLocation(this->basic_shader->Program, "u_texture"), 0);

		m_pTrack->money[obstacle].money_obj[0]->draw();
	}
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
};

void TrainView::
drawSkybox()
{
	glm::mat4 skybox_view_matrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, &skybox_view_matrix[0][0]);
	skybox_view_matrix = glm::mat4(glm::mat3(skybox_view_matrix));
	glm::mat4 skybox_projection_matrix;
	glGetFloatv(GL_PROJECTION_MATRIX, &skybox_projection_matrix[0][0]);

	glBindBuffer(GL_UNIFORM_BUFFER, this->skybox_matrices->ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &skybox_projection_matrix[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &skybox_view_matrix[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skybox_shader->Use();
	glUniform1i(glGetUniformLocation(this->skybox_shader->Program, "skybox"), 0);
	// skybox cube
	glBindVertexArray(this->skybox->vertex_data->vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture[chapter]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

void TrainView::
drawDoor()
{
	if (camera_movement_state == 1) return; // don't draw the door after beginning camera movement
	if (!load_door_position) // maintain the position in front of the player (setting in the beginning)
	{
		load_door_position = true;
		door_pos = player_pos - 10.0f * player_forward - 11.0f * player_up;
		door_forward = player_forward + player_up;
		door_up = player_up - player_forward;
	}

	this->door_shader->Use();
	mat4 model_matrix = inverse(lookAt(door_pos, door_pos + door_forward, door_up)); // the player is in a 5.0f height position
	model_matrix = scale(model_matrix, vec3(25, 25, 25));
	glUniformMatrix4fv(glGetUniformLocation(this->basic_shader->Program, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	door_scene_texture->bind(0);
	glUniform1i(glGetUniformLocation(this->door_shader->Program, "door_scene_texture"), 0); //a picture within green on the door
	door_texture->bind(1);
	glUniform1i(glGetUniformLocation(this->door_shader->Program, "door_texture"), 1); //a picture include the door
	glUniform1f(glGetUniformLocation(this->door_shader->Program, "offset"), door_offset); //the door move between [-0.5, 0] in texture coordinate
	glBindVertexArray(this->door->vertex_data->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

void TrainView::
drawScreenQuad()
{
	this->screen_shader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screen_id);
	glUniform1i(glGetUniformLocation(this->screen_shader->Program, "screen"), 0);
	glUniform1f(glGetUniformLocation(this->screen_shader->Program, "brightness"), screen_brightness);

	glBindVertexArray(this->screen_quad->vao);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

//************************************************************************
//
// * this is the code that actually draws the window
//   it puts a lot of the work into other routines to simplify things
//========================================================================
void TrainView::
draw()
{

	//*********************************************************************
	//
	// * Set up basic opengl informaiton
	//
	//**********************************************************************
	//initialized glad
	if (gladLoadGL())
	{
		//initiailize VAO, VBO, Shader...

		if (!this->basic_shader)
			this->basic_shader = new
			Shader(
				"../GraphicsRush/src/shaders/simple.vert",
				nullptr, nullptr, nullptr,
				"../GraphicsRush/src/shaders/simple.frag");

		if (!this->skybox_shader)
			this->skybox_shader = new
			Shader(
				"../GraphicsRush/src/shaders/skybox.vert",
				nullptr, nullptr, nullptr,
				"../GraphicsRush/src/shaders/skybox.frag");

		if (!this->text_shader)
			this->text_shader = new
			Shader(
				"../GraphicsRush/src/shaders/text.vert",
				nullptr, nullptr, nullptr,
				"../GraphicsRush/src/shaders/text.frag");

		if (!this->screen_shader)
			this->screen_shader = new
			Shader(
				"../GraphicsRush/src/shaders/screen.vert",
				nullptr, nullptr, nullptr,
				"../GraphicsRush/src/shaders/screen.frag");

		if (!this->door_shader)
			this->door_shader = new
			Shader(
				"../GraphicsRush/src/shaders/door.vert",
				nullptr, nullptr, nullptr,
				"../GraphicsRush/src/shaders/door.frag");

		if (!this->commom_matrices)
		{
			this->commom_matrices = new UBO();
			glGenBuffers(1, &this->commom_matrices->ubo);
		}
		this->commom_matrices->size = 2 * sizeof(glm::mat4);
		glBindBuffer(GL_UNIFORM_BUFFER, this->commom_matrices->ubo);
		glBufferData(GL_UNIFORM_BUFFER, this->commom_matrices->size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		if (!this->skybox_matrices)
		{
			this->skybox_matrices = new UBO();
			glGenBuffers(1, &this->skybox_matrices->ubo);
		}
		this->skybox_matrices->size = 2 * sizeof(glm::mat4);
		glBindBuffer(GL_UNIFORM_BUFFER, this->skybox_matrices->ubo);
		glBufferData(GL_UNIFORM_BUFFER, this->skybox_matrices->size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		/*********************NEW ADDITIONS*********************/
		if (!this->path) this->path = new ShaderInfo;
		//add info for each track between control points

		if (!load_chapter)
		{
			tw->m_Track.readPoints(chapter_path_file[chapter].c_str());
			initCameraMovement();
			load_chapter = true;
		}

		initPath();

		if (!this->path_texture)
			this->path_texture = new Texture2D("../GraphicsRush/Images/tracks/default.png");

		if (!this->player_texture)
			this->player_texture = new Texture2D(player_texture_path.c_str());

		if (!this->device) {
			//Tutorial: https://ffainelli.github.io/openal-example/
			this->device = alcOpenDevice(NULL);
			if (!this->device)
				puts("ERROR::NO_AUDIO_DEVICE");

			ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
			if (enumeration == AL_FALSE)
				puts("Enumeration not supported");
			else
				puts("Enumeration supported");

			this->context = alcCreateContext(this->device, NULL);
			if (!alcMakeContextCurrent(context))
				puts("Failed to make context current");

			this->source_pos = glm::vec3(0.0f, 0.0f, 0.0f);

			ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
			alListener3f(AL_POSITION, source_pos.x, source_pos.y, source_pos.z);
			alListener3f(AL_VELOCITY, 0, 0, 0);
			alListenerfv(AL_ORIENTATION, listenerOri);

			alGenSources((ALuint)1, &this->source);
			alSourcef(this->source, AL_PITCH, 1);
			alSourcef(this->source, AL_GAIN, 1.0f);
			alSource3f(this->source, AL_POSITION, source_pos.x, source_pos.y, source_pos.z);
			alSource3f(this->source, AL_VELOCITY, 0, 0, 0);
			alSourcei(this->source, AL_LOOPING, AL_TRUE);

			alGenBuffers((ALuint)1, &this->buffer);

			ALsizei size, freq;
			ALenum format;
			ALvoid* data;
			ALboolean loop = AL_TRUE;

			//Material from: ThinMatrix
			alutLoadWAVFile((ALbyte*)"../GraphicsRush/Audios/wholesome.wav", &format, &data, &size, &freq, &loop);
			alBufferData(this->buffer, format, data, size, freq);
			alSourcei(this->source, AL_BUFFER, this->buffer);

			if (format == AL_FORMAT_STEREO16 || format == AL_FORMAT_STEREO8)
				puts("TYPE::STEREO");
			else if (format == AL_FORMAT_MONO16 || format == AL_FORMAT_MONO8)
				puts("TYPE::MONO");

			alSourcePlay(this->source);

			//money sound
			alGenSources((ALuint)1, &this->moneySource);
			alSourcef(this->moneySource, AL_PITCH, 1);
			alSourcef(this->moneySource, AL_GAIN, 1.0f);
			alSource3f(this->moneySource, AL_POSITION, source_pos.x, source_pos.y, source_pos.z);
			alSource3f(this->moneySource, AL_VELOCITY, 0, 0, 0);
			alSourcei(this->moneySource, AL_LOOPING, AL_FALSE);

			alGenBuffers((ALuint)1, &this->moneyBuffer);

			loop = AL_FALSE;

			//Material from: ThinMatrix
			alutLoadWAVFile((ALbyte*)"../GraphicsRush/Audios/bounce.wav", &format, &data, &size, &freq, &loop);
			alBufferData(this->moneyBuffer, format, data, size, freq);
			alSourcei(this->moneySource, AL_BUFFER, this->moneyBuffer);

			if (format == AL_FORMAT_STEREO16 || format == AL_FORMAT_STEREO8)
				puts("TYPE::STEREO");
			else if (format == AL_FORMAT_MONO16 || format == AL_FORMAT_MONO8)
				puts("TYPE::MONO");

			// cleanup context
			//alDeleteSources(1, &source);
			//alDeleteBuffers(1, &buffer);
			//device = alcGetContextsDevice(context);
			//alcMakeContextCurrent(NULL);
			//alcDestroyContext(context);
			//alcCloseDevice(device);
		}

		if (!player_obj)
			player_obj = new Model(player_obj_path);

		if (!door_scene_texture)
			door_scene_texture = new Texture2D(door_scene_texture_path.c_str());

		if (!door_texture)
			door_texture = new Texture2D(door_texture_path.c_str());

		initDoor();

		if (!cubemap_texture_load)
		{
			cubemap_texture_load = true;
			cubemap_texture.push_back(loadCubemap(chapters_skybox_textures_faces[0])); // chapter 1
			cubemap_texture.push_back(loadCubemap(chapters_skybox_textures_faces[1])); // chapter 2
			cubemap_texture.push_back(loadCubemap(chapters_skybox_textures_faces[2])); // chapter 3
			cubemap_texture.push_back(loadCubemap(chapters_skybox_textures_faces[3])); // chapter 4
			cubemap_texture.push_back(loadCubemap(chapters_skybox_textures_faces[4])); // chapter 5
		}

		if (!this->skybox) this->skybox = new ShaderInfo;

		initSkybox();

		if (!font_isloaded) font_isloaded = initText();

		initScreenRender();

		initScreenQuad();
	}
	else
		throw std::runtime_error("Could not initialize GLAD!");

	// Set up the view port
	glViewport(0, 0, w(), h());

	// clear the window, be sure to clear the Z-Buffer too
	glClearColor(0, 0, .3f, 0);		// background should be blue

	// we need to clear out the stencil buffer since we'll use
	// it for shadows
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);

	// Blayne prefers GL_DIFFUSE
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();		// put the code to set up matrices here

	//######################################################################
	// TODO: 
	// you might want to set the lighting up differently. if you do, 
	// we need to set up the lights AFTER setting up the projection
	//######################################################################
	// enable the lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	// set linstener position 
	if (selectedCube >= 0)
		alListener3f(AL_POSITION,
			m_pTrack->points[selectedCube].pos.x,
			m_pTrack->points[selectedCube].pos.y,
			m_pTrack->points[selectedCube].pos.z);
	else
		alListener3f(AL_POSITION,
			this->source_pos.x,
			this->source_pos.y,
			this->source_pos.z);


	//*********************************************************************
	// now draw the ground plane
	//*********************************************************************
	// set to opengl fixed pipeline(use opengl 1.x draw function)
	glUseProgram(0);

	//setupFloor();
	//glDisable(GL_LIGHTING);
	//drawFloor(200, 10);


	//*********************************************************************
	// now draw the object and we need to do it twice
	// once for real, and then once for shadows
	//*********************************************************************
	//glEnable(GL_LIGHTING);

	setupObjects();

	renderScreenBegin();

	drawStuff();

	// this time drawing is for shadows (except for top view)
	if (!tw->topCam->value()) {
		setupShadows();
		drawStuff(true);
		unsetupShadows();
	}

	setUBO();
	glBindBufferRange(
		GL_UNIFORM_BUFFER, /*binding point*/0, this->commom_matrices->ubo, 0, this->commom_matrices->size);
	glBindBufferRange(
		GL_UNIFORM_BUFFER, /*binding point*/1, this->skybox_matrices->ubo, 0, this->skybox_matrices->size);

	drawPath();

	drawPlayer();

	drawDoor();

	//use money to check if world is loaded
	if ((int)m_pTrack->money.size() == 0) loadObjects();

	drawObstacles();
	drawMoney();

	drawSkybox();

	char score_info[20];
	sprintf(score_info, "Score:  %010d", m_pTrack->score);
	RenderText(score_info, 25.0f, h() - 30.0f, 0.6f, vec3(0.9f, 0.9f, 0.9f));

	char money_info[20];
	sprintf(money_info, "money: %010d", m_pTrack->money_collected);
	RenderText(money_info, 25.0f, h() - 55.0f, 0.6f, vec3(0.9f, 0.9f, 0.9f));

	renderScreenEnd();

	drawScreenQuad();
}

//************************************************************************
//
// * This sets up both the Projection and the ModelView matrices
//   HOWEVER: it doesn't clear the projection first (the caller handles
//   that) - its important for picking
//========================================================================
void TrainView::
setProjection()
//========================================================================
{
	// Compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(w()) / static_cast<float>(h());

	// Check whether we use the world camp
	if (tw->worldCam->value())
		arcball.setProjection(false);
	// Or we use the top cam
	else if (tw->topCam->value()) {
		float wi, he;
		if (aspect >= 1) {
			wi = 110;
			he = wi / aspect;
		}
		else {
			he = 110;
			wi = he * aspect;
		}

		// Set up the top camera drop mode to be orthogonal and set
		// up proper projection matrix
		glMatrixMode(GL_PROJECTION);
		glOrtho(-wi, wi, -he, he, 200, -200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-90, 1, 0, 0);
	}
	// Or do the train view or other view here
	//####################################################################
	// TODO: 
	// put code for train view projection here!	
	//####################################################################
	else if (tw->trainCam->value()) {

		//some OpenGL stuff
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//set perspective
		float aspect = (float)w() / h();
		gluPerspective(100, aspect, 1, 200);

		//more OpenGL stuff
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (camera_movement_state == 0)
			cameraMovement();
		else
		{
			//prepare variables
			float ratio = m_pTrack->trainU - (int)m_pTrack->trainU;
			int cp_id = (int)tw->m_Track.trainU;
			vec3 orient, crossed, thisPosition, nextPosition;
			//find trainPosition, forward and orient
			gmt.setG_pos(cp_id);
			train_pos = gmt.calculate(ratio);

			nextPosition = gmt.calculate(ratio + 1.0f / PATH_DIVIDE);
			forward = vec3(nextPosition - train_pos);

			gmt.setG_orient(cp_id);
			thisPosition = gmt.calculate(ratio);
			nextPosition = gmt.calculate(ratio + 1.0f / PATH_DIVIDE);
			orient = (1.0f - ratio) * thisPosition + ratio * nextPosition;

			//find playerUp (the orient perpendicular to the rail)
			crossed = cross(forward, orient);
			up = cross(crossed, forward);

			//normalize all vec3s for use
			forward = normalize(forward);
			orient = normalize(orient);
			up = normalize(up);
			crossed = normalize(crossed);
			//set look at (trainPosition(viewerPosition) -> where to look at -> up)
			vec3 viewer_pos = train_pos + up * 20.0f - forward * 10.0f;
			if (abs(m_pTrack->switchLane - (float)m_pTrack->lane) > 0.01) {
				if (m_pTrack->switchLane < (float)m_pTrack->lane) m_pTrack->switchLane += 0.1f;
				else if (m_pTrack->switchLane > (float)m_pTrack->lane) m_pTrack->switchLane -= 0.1f;
			}
			viewer_pos = viewer_pos + (float)m_pTrack->switchLane * crossed * 5.0f;
			if (m_pTrack->jumpingState != -1) {
				viewer_pos = viewer_pos + m_pTrack->airbornePosition[m_pTrack->jumpingState] * up * 10.0f;
				m_pTrack->jumpingState++;
		}
			if (m_pTrack->jumpingState == (int)m_pTrack->airbornePosition.size()) m_pTrack->jumpingState = -1;
			gluLookAt(viewer_pos.x, viewer_pos.y, viewer_pos.z,
				viewer_pos.x + forward.x * 30.0f - up.x * 20.0f,
				viewer_pos.y + forward.y * 30.0f - up.y * 20.0f,
				viewer_pos.z + forward.z * 30.0f - up.z * 20.0f,
				up.x, up.y, up.z);
		}


#ifdef EXAMPLE_SOLUTION
		trainCamView(this, aspect);
#endif
	}
}

//************************************************************************
//
// * this draws all of the stuff in the world
//
//	NOTE: if you're drawing shadows, DO NOT set colors (otherwise, you get 
//       colored shadows). this gets called twice per draw 
//       -- once for the objects, once for the shadows
//########################################################################
// TODO: 
// if you have other objects in the world, make sure to draw them
//########################################################################
//========================================================================
void TrainView::drawStuff(bool doingShadows)
{
	// Draw the control points
	// don't draw the control points if you're driving 
	// (otherwise you get sea-sick as you drive through them)
	if (!tw->trainCam->value()) {
		for (size_t i = 0; i < m_pTrack->points.size(); ++i) {
			if (!doingShadows) {
				if (((int)i) != selectedCube)
					glColor3ub(240, 60, 60);
				else
					glColor3ub(240, 240, 30);
			}
			m_pTrack->points[i].draw();
		}
	}
	// draw the track
	//####################################################################
	// TODO: 
	// call your own track drawing code
	//####################################################################

#ifdef EXAMPLE_SOLUTION
	drawTrack(this, doingShadows);
#endif

	// draw the train
	//####################################################################
	// TODO: 
	//	call your own train drawing code
	//####################################################################
#ifdef EXAMPLE_SOLUTION
	// don't draw the train if you're looking out the front window
	if (!tw->trainCam->value())
		drawTrain(this, doingShadows);
#endif
}

// 
//************************************************************************
//
// * this tries to see which control point is under the mouse
//	  (for when the mouse is clicked)
//		it uses OpenGL picking - which is always a trick
//########################################################################
// TODO: 
//		if you want to pick things other than control points, or you
//		changed how control points are drawn, you might need to change this
//########################################################################
//========================================================================
void TrainView::
doPick()
//========================================================================
{
	// since we'll need to do some GL stuff so we make this window as 
	// active window
	make_current();

	// where is the mouse?
	int mx = Fl::event_x();
	int my = Fl::event_y();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Set up the pick matrix on the stack - remember, FlTk is
	// upside down!
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix((double)mx, (double)(viewport[3] - my),
		5, 5, viewport);

	// now set up the projection
	setProjection();

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100, buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	// draw the cubes, loading the names as we go
	for (size_t i = 0; i < m_pTrack->points.size(); ++i) {
		glLoadName((GLuint)(i + 1));
		m_pTrack->points[i].draw();
	}

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selectedCube = buf[3] - 1;
	}
	else // nothing hit, nothing selected
		selectedCube = -1;

	printf("Selected Cube %d\n", selectedCube);
}

void TrainView::setUBO()
{
	float wdt = this->pixel_w();
	float hgt = this->pixel_h();

	glm::mat4 view_matrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, &view_matrix[0][0]);
	//HMatrix view_matrix; 
	//this->arcball.getMatrix(view_matrix);

	glm::mat4 projection_matrix;
	glGetFloatv(GL_PROJECTION_MATRIX, &projection_matrix[0][0]);
	//projection_matrix = glm::perspective(glm::radians(this->arcball.getFoV()), (GLfloat)wdt / (GLfloat)hgt, 0.01f, 1000.0f);

	glBindBuffer(GL_UNIFORM_BUFFER, this->commom_matrices->ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection_matrix[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view_matrix[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// render line of text
// -------------------
void TrainView::
RenderText(std::string text, float x, float y, float scale, vec3 color)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// activate corresponding render state	
	text_shader->Use();
	glm::mat4 projection = glm::ortho(0.0f, (float)w(), 0.0f, (float)h());
	glUniformMatrix4fv(glGetUniformLocation(text_shader->Program, "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniform3fv(glGetUniformLocation(text_shader->Program, "text_color"), 1, &color[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font_vertex_data->vao);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, font_vertex_data->vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

std::string TrainView::
getFileName(std::string file_path)
{
	std::stringstream ss(file_path);
	std::string file_name, file_name_t;
	while (getline(ss, file_name_t, '/')) file_name = file_name_t;
	return file_name;
}