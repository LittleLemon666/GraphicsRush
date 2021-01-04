/************************************************************************
     File:        TrainView.H

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

#pragma once

#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Shadow.h"
#include "RenderUtilities/Environment.h"

// Preclarify for preventing the compiler error
class TrainWindow;
class CTrack;

/*********************NEW ADDITIONS*********************/
#include <vector>
#include "ShaderInfo.h"
#include "GMTPipeline.h"
#include "Model.H"
#include "Boss.H"
#include "Shop.H"
#include <ft2build.h>
#include <freetype/freetype.h>

//#######################################################################
// TODO
// You might change the TrainView in order to add different objects to
// be drawn, or adjust the lighting, or ...
//#######################################################################


// The TrainView also needs to handle its user interface events.
// since this is an FLTk Window, we need to include FlTk Headers
// the pragmas turn off the warnings from FlTk - our code should have no warnings,
// but their code doesn't meet that standard
#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <Fl/Fl_Gl_Window.h>
#pragma warning(pop)

#include <AL/alut.h>
//#include <AL/al.h>
//#include <AL/alc.h>

// this uses the old ArcBall Code
#include "Utilities/ArcBallCam.H"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

enum Chooser
{
	CLOBBY, // 0 is back ground in chooser
	CSHOP,
	CGAME,
	CDEAD,
	CFREE,
	CVER2,
	CVER3,
	CSHADER,
	CCUDA,
	CCHECKPOINT1,
	CCHECKPOINT2,
	CCHECKPOINT3,
	CCHECKPOINT4,
	CCHECKPOINT5
};

enum Lighting
{
	LDIR,	// only directional light
	LPOINT	// only point light
};

class TrainView : public Fl_Gl_Window
{
	public:
		// note that we keep the "standard widget" constructor arguments
		TrainView(int x, int y, int w, int h, const char* l = 0);

		// overrides of important window things
		virtual int handle(int);
		virtual void draw();

		// all of the actual drawing happens in this routine
		// it has to be encapsulated, since we draw differently if
		// we're drawing shadows (no colors, for example)
		void drawStuff(bool doingShadows=false);

		// setup the projection - assuming that the projection stack has been
		// cleared for you
		void setProjection();

		// Reset the Arc ball control
		void resetArcball();

		// pick a point (for when the mouse goes down)
		void doPick();

		//set ubo
		void setUBO();

		bool load_chapter = false;
	public:
		ArcBallCam		arcball;			// keep an ArcBall for the UI
		int				selectedCube = -1;  // simple - just remember which cube is selected

		TrainWindow*	tw;				// The parent of this display window
		CTrack*			m_pTrack;		// The track of the entire scene

		Shader* basic_shader		= nullptr;
		Texture2D* path_texture	= nullptr;
		UBO* commom_matrices = nullptr; // bind 0
		UBO* skybox_matrices = nullptr; // bind 1
		UBO* camera_properties = nullptr; // bind 2
		UBO* dir_light_properties = nullptr; // bind 3
		UBO* point_light_properties = nullptr; // bind 4

		//OpenAL
		glm::vec3 source_pos;
		glm::vec3 listener_pos;
		ALCdevice* device	= nullptr;
		ALCcontext* context = nullptr;
		ALuint source;
		ALuint buffer;
		ALuint moneySource;
		ALuint moneyBuffer;

		/*********************NEW ADDITIONS*********************/
		ShaderInfo* path = nullptr;
		ShaderInfo* skybox = nullptr;
		ShaderInfo* door = nullptr;
		Shader* door_shader = nullptr;
		Shader* skybox_shader = nullptr;
		Shader* text_shader = nullptr;
		Shader* screen_shader = nullptr;
		Shader* water_surface_shader = nullptr;
		Shader* shadow_shader = nullptr;
		Shader* choose_shader = nullptr;
		Shader* choose_flat_shader = nullptr;
		Shader* environment_shader = nullptr;
		Shader* blending_shader = nullptr;
		Shader* blending_flat_shader = nullptr;
		Shader* blending_flat_gray_shader = nullptr;
		Shader* firework_shader = nullptr;
		void loadObjects();
		void loadMiniBoss();
		void loadMainBoss();
		void loadExtraBoss();
		void initPath();
		void initSkybox();
		void initDoor();
		bool initText();
		void initCameraMovement();
		void cameraMovement();
		void initScreenRender();
		void renderScreenBegin();
		void renderScreenEnd();
		void renderDepthMapBegin();
		void renderDepthMapEnd(); //may be fault in unsetupShadows
		void setLightSpaceMatrix();
		void initScreenQuad();
		void initMultiBall();
		void setDirLightUBO();
		void setPointLightUBO();
		void switchLightMode();
		void setCameraUBO();
		void renderChooseBegin();
		void renderChooseEnd();
		void choose(int x, int y);
		int decodeChoose(vec3 uv);
		void renderEnvironment();
		void drawShop(bool buttom = false);
		void drawChooser();
		void drawWorld();
		void drawPath(bool doShadow = false);
		void drawPlayer(bool doShadow = false);
		void drawIcon();
		void drawObstacles(bool doShadow = false);
		void drawMoney(bool doShadow = false);
		void drawMiniBoss();
		void drawMainBoss();
		void drawMultiBall();
		void drawExtraBoss();
		void drawReversiPiece(bool doShadow = false);
		void drawSkybox();
		void drawDoor();
		void drawEarth();
		void drawSun();
		void drawFree(bool buttom = false);
		void drawVer2(bool buttom = false);
		void drawVer3(bool buttom = false);
		void drawShader(bool buttom = false);
		void drawCuda(bool buttom = false);
		void drawCheckpoint(bool buttom = false);
		void drawScreenQuad();
		void printText();
		vec2 ndcToViewport(const vec3& ndc);
		GLuint loadCubemap(std::vector<std::string> chapters_skybox_textures_faces);
		const int PATH_DIVIDE = 50;
		GMTPipeline gmt;

		vec3 train_pos, forward, up;
		vec3 player_pos, player_forward, player_up;
		vec3 viewer_pos;

		Model* player_obj = nullptr;
		std::string player_obj_path = "../GraphicsRush/Objects/pikachu.obj";
		Texture2D* player_texture = nullptr;
		std::string player_texture_path = "../GraphicsRush/Objects/Pikachu.png";

		std::string obstacle_obj_path = "../GraphicsRush/Objects/box.obj";
		std::string obstacle_texture_path = "../GraphicsRush/Objects/box.jpg";

		std::string money_obj_path = "../GraphicsRush/Objects/coin.obj";
		std::string money_texture_path = "../GraphicsRush/Objects/coin_texture.jpg";
		float money_rotate = 0;

		int camera_movement_state = 0;
		int camera_movement_index = 0;
		std::vector<std::vector<vec3>> camera_movement;
		int chapter = 0;
		const int NUMBER_OF_PROJECTS = 5;
		bool load_game_objects = false;
		std::vector<std::string> chapter_path_file;
		std::vector<std::string> chapter_path_file_name;
		std::string getFileName(std::string);

		Texture2D* door_scene_texture = nullptr;
		std::string door_scene_texture_path = "../GraphicsRush/Images/lb001_door_scene.jpg";
		Texture2D* door_texture = nullptr;
		std::string door_texture_path = "../GraphicsRush/Images/lb001_door.jpg";
		bool load_door_position = false;
		vec3 door_pos, door_forward, door_up;
		float door_offset = 0.0f;

		std::vector<GLuint> cubemap_texture;
		std::vector<std::vector<std::string>> chapters_skybox_textures_faces;
		bool cubemap_texture_load = false;
		void switchChapter(const int&);

		bool font_isloaded = false;
		std::string font_name = "../GraphicsRush/fonts/54151___.TTF";
		std::map<GLchar, Character> Characters;
		VAO* font_vertex_data = nullptr;
		void RenderText(std::string text, float x, float y, float scale, vec3 color);

		VAO* screen_quad = nullptr;
		FBO* screen_FBO = nullptr;
		GLuint screen_id;
		bool load_screen = false;
		float screen_brightness = 1.0f;

		struct DirLight
		{
			float shininess = 64.0f;
			vec3 light_direction = vec3(1.0, -1.0, 1.0);
			vec3 light_ambient = vec3(0.8, 0.8, 0.8);
			vec3 light_diffuse = vec3(0.95, 0.95, 0.95);
			vec3 light_specular = vec3(1.0, 1.0, 1.0);
		}dir_light;

		struct PointLight
		{
			float shininess = 64.0f;
			float constant = 1.0f;
			float linear = 0.09f;
			float quadratic = 0.0032f;
			vec3 light_position;
			vec3 light_ambient = vec3(0.8f, 0.8f, 0.8f);
			vec3 light_diffuse = vec3(0.95f, 0.95f, 0.95f);
			vec3 light_specular = vec3(1.0f, 1.0f, 1.0f);
		}point_light;

		vec3 camera_pos;

		Shadow* shadow = nullptr;
		mat4 lightProjection, lightView;
		mat4 lightSpaceMatrix;

		Shop* shop = nullptr;
		float shop_rotate = 0;

		FBO* chooser_FBO = nullptr;
		int game_state = CLOBBY;

		Model* earth_obj = nullptr;
		std::string earth_obj_path = "SPHERE";
		Texture2D* earth_texture = nullptr;
		std::string earth_texture_path = "../GraphicsRush/Objects/earth.bmp";
		float earth_rotate = 0;

		Model* sun_obj = nullptr;
		std::string sun_obj_path = "SPHERE";
		Texture2D* sun_texture = nullptr;
		std::string sun_texture_path = "../GraphicsRush/Objects/sun.jpg";
		float sun_rotate = 0;
		void rotate_objects();

		vec3 sun_pos = vec3(-580.0f, 50.0f, 530.0f);
		int light_mode = Lighting::LDIR;

		Environment* environment = nullptr;
		FBO* environment_FBO = nullptr;
		bool getting_environment = false;

		vec3 multiBallPos, multiBallForward, multiBallUp, multiBallCross;
		Model* multiball_obj = nullptr;
		std::string multiball_obj_path = "SPHERE";

		Model* mini_boss_obj = nullptr;
		std::string mini_boss_obj_path = "QUAD";
		Texture2D* mini_boss_obj_texture = nullptr;
		std::string mini_boss_obj_texture_path = "../GraphicsRush/Images/boss/miniboss.png";
};