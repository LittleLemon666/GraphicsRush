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
int TrainView::handle(int event)
{
	// see if the ArcBall will handle the event - if it does, 
	// then we're done
	// note: the arcball only gets the event if we're in world view
	if (tw->worldCam->value())
		if (arcball.handle(event))
			return 1;

	// remember what button was used
	static int last_push;

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
		if (k == 'a' && m_pTrack->lane > -1) {
			m_pTrack->lane--;
			return 1;
		}
		if (k == 'd' && m_pTrack->lane < 1) {
			m_pTrack->lane++;
			return 1;
		}
		if (k == 'w' && m_pTrack->jumpingState == -1) {
			m_pTrack->jumpingState = 0;
			return 1;
		}
		break;
	}

	return Fl_Gl_Window::handle(event);
}

/*********************NEW ADDITIONS*********************/
void TrainView::initPath() {
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

void TrainView::drawPath() {
	//bind shader
	this->path_shader->Use();

	glm::mat4 model_matrix = glm::mat4();
	model_matrix = glm::translate(model_matrix, this->source_pos);
	glUniformMatrix4fv(glGetUniformLocation(this->path_shader->Program, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glUniform3fv(glGetUniformLocation(this->path_shader->Program, "u_color"), 1, &glm::vec3(0.0f, 1.0f, 0.0f)[0]);
	this->path_texture->bind(0);
	glUniform1i(glGetUniformLocation(this->path_shader->Program, "u_texture"), 0);

	//bind VAO
	glBindVertexArray(this->path->vertex_data->vao);

	glDrawElements(GL_TRIANGLES, this->path->vertex_data->element_amount, GL_UNSIGNED_INT, 0);

	//unbind VAO
	glBindVertexArray(0);

	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
};

//************************************************************************
//
// * this is the code that actually draws the window
//   it puts a lot of the work into other routines to simplify things
//========================================================================
void TrainView::draw()
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

		if (!this->path_shader)
			this->path_shader = new
			Shader(
				"../GraphicsRush/src/shaders/simple.vert",
				nullptr, nullptr, nullptr,
				"../GraphicsRush/src/shaders/simple.frag");

		if (!this->commom_matrices)
		{
			this->commom_matrices = new UBO();
			glGenBuffers(1, &this->commom_matrices->ubo);
		}
		this->commom_matrices->size = 2 * sizeof(glm::mat4);
		glBindBuffer(GL_UNIFORM_BUFFER, this->commom_matrices->ubo);
		glBufferData(GL_UNIFORM_BUFFER, this->commom_matrices->size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		/*********************NEW ADDITIONS*********************/
		if (!this->path) this->path = new ShaderInfo;
		//add info for each track between control points
		initPath();

		if (!this->path_texture)
			this->path_texture = new Texture2D("../GraphicsRush/Images/tracks/default.png");

		if (!this->pikachu_texture)
			this->pikachu_texture = new Texture2D("../GraphicsRush/Objects/Pikachu.png");

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

			this->source_pos = glm::vec3(0.0f, 5.0f, 0.0f);

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
			alutLoadWAVFile((ALbyte*)"../GraphicsRush/Audios/bounce.wav", &format, &data, &size, &freq, &loop);
			alBufferData(this->buffer, format, data, size, freq);
			alSourcei(this->source, AL_BUFFER, this->buffer);

			if (format == AL_FORMAT_STEREO16 || format == AL_FORMAT_STEREO8)
				puts("TYPE::STEREO");
			else if (format == AL_FORMAT_MONO16 || format == AL_FORMAT_MONO8)
				puts("TYPE::MONO");

			alSourcePlay(this->source);

			// cleanup context
			//alDeleteSources(1, &source);
			//alDeleteBuffers(1, &buffer);
			//device = alcGetContextsDevice(context);
			//alcMakeContextCurrent(NULL);
			//alcDestroyContext(context);
			//alcCloseDevice(device);
		}

		if (!pikachu)
			pikachu = new Object(pikachu_obj);
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

	setupFloor();
	//glDisable(GL_LIGHTING);
	drawFloor(200, 10);


	//*********************************************************************
	// now draw the object and we need to do it twice
	// once for real, and then once for shadows
	//*********************************************************************
	//glEnable(GL_LIGHTING);
	setupObjects();

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
	drawPath();

	//bind shader
	this->path_shader->Use();

	glm::mat4 model_matrix = glm::mat4();
	model_matrix = glm::translate(model_matrix, glm::vec3(0, 1, 0));
	model_matrix = glm::scale(model_matrix, glm::vec3(10, 10, 10));
	glUniformMatrix4fv(glGetUniformLocation(this->path_shader->Program, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glUniform3fv(glGetUniformLocation(this->path_shader->Program, "u_color"), 1, &glm::vec3(0.0f, 1.0f, 0.0f)[0]);
	this->pikachu_texture->bind(1);
	glUniform1i(glGetUniformLocation(this->path_shader->Program, "u_texture"), 1);

	pikachu->draw();

	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
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

		//prepare variables
		float ratio = m_pTrack->trainU - (int)m_pTrack->trainU;
		int cp_id = (int)tw->m_Track.trainU;
		vec3 trainPosition, forward, orient, crossed, up, thisPosition, nextPosition;
		//find trainPosition, forward and orient
		gmt.setG_pos(cp_id);
		trainPosition = gmt.calculate(ratio);

		nextPosition = gmt.calculate(ratio + 1.0f / PATH_DIVIDE);
		forward = vec3(nextPosition - trainPosition);

		gmt.setG_orient(cp_id);
		thisPosition = gmt.calculate(ratio);
		nextPosition = gmt.calculate(ratio + 1.0f / PATH_DIVIDE);
		orient = (1.0f - ratio) * thisPosition + ratio * nextPosition;
		
		//find up (the orient perpendicular to the rail)
		crossed = cross(forward, orient);
		up = cross(crossed, forward);

		//normalize all vec3s for use
		forward = normalize(forward);
		orient = normalize(orient);
		up = normalize(up);
		crossed = normalize(crossed);
		printf("%d\n", m_pTrack->lane);
		//set look at (trainPosition(viewerPosition) -> where to look at -> up)
		vec3 viewer_pos = trainPosition + up * 10.0f - forward * 10.0f;
		if (abs(m_pTrack->switchLane - (float)m_pTrack->lane) > 0.01) {
			if (m_pTrack->switchLane < (float)m_pTrack->lane) m_pTrack->switchLane += 0.1f;
			else if (m_pTrack->switchLane > (float)m_pTrack->lane) m_pTrack->switchLane -= 0.1f;
		}
		if (m_pTrack->jumpingState == (int)m_pTrack->airbornePosition.size()) m_pTrack->jumpingState = -1;
		viewer_pos = viewer_pos + (float)m_pTrack->switchLane * crossed * 5.0f;
		if (m_pTrack->jumpingState != -1) {
			viewer_pos = viewer_pos + m_pTrack->airbornePosition[m_pTrack->jumpingState] * up * 10.0f;
			m_pTrack->jumpingState++;
		}
		gluLookAt(viewer_pos.x, viewer_pos.y, viewer_pos.z,
			viewer_pos.x + forward.x,
			viewer_pos.y + forward.y,
			viewer_pos.z + forward.z,
			up.x, up.y, up.z);

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