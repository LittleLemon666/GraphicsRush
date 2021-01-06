/************************************************************************
	 File:        CallBacks.H
	 Author:
				  Michael Gleicher, gleicher@cs.wisc.edu
	 Modifier
				  Yu-Chi Lai, yu-chi@cs.wisc.edu

	 Comment:     Header file to define callback functions.
						define the callbacks for the TrainWindow
						these are little functions that get called when the
						various widgets
						get accessed (or the fltk timer ticks). these
						functions are used
						when TrainWindow sets itself up.
	 Platform:    Visio Studio.Net 2003/2005
*************************************************************************/
#pragma once

#include <time.h>
#include <math.h>
#include <sstream>
#include <vector>

#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"
#include "Boss.H"
#include "Shop.H"
#include "Object.H"

#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <Fl/Fl_File_Chooser.H>
#include <Fl/math.h>
#pragma warning(pop)

//***************************************************************************
//
// * Reset the control points back to their base setup
//===========================================================================
void resetCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	tw->m_Track.resetPoints();
	tw->trainView->selectedCube = -1;
	tw->m_Track.trainU = 0;
	tw->damageMe();
}

//***************************************************************************
//
// * any time something changes, you need to force a redraw
//===========================================================================
void damageCB(Fl_Widget*, TrainWindow* tw)
{
	tw->damageMe();
}

//***************************************************************************
//
// * Callback that adds a new point to the spline
// idea: add the point AFTER the selected point
//===========================================================================
void addPointCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	// get the number of points
	size_t npts = tw->m_Track.points.size();
	// the number for the new point
	size_t newidx = (tw->trainView->selectedCube >= 0) ? tw->trainView->selectedCube : 0;

	// pick a reasonable location
	size_t previdx = (newidx + npts - 1) % npts;
	Pnt3f npos = (tw->m_Track.points[previdx].pos + tw->m_Track.points[newidx].pos) * .5f;

	tw->m_Track.points.insert(tw->m_Track.points.begin() + newidx, npos);

	// make it so that the train doesn't move - unless its affected by this control point
	// it should stay between the same points
	if (ceil(tw->m_Track.trainU) > ((float)newidx)) {
		tw->m_Track.trainU += 1;
		if (tw->m_Track.trainU >= npts) tw->m_Track.trainU -= npts;
	}

	tw->damageMe();
}

//***************************************************************************
//
// * Callback that deletes a point from the spline
//===========================================================================
void deletePointCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	if (tw->m_Track.points.size() > 4) {
		if (tw->trainView->selectedCube >= 0) {
			tw->m_Track.points.erase(tw->m_Track.points.begin() + tw->trainView->selectedCube);
		}
		else
			tw->m_Track.points.pop_back();
	}
	tw->damageMe();
}
//***************************************************************************
//
// * Advancing the train
//===========================================================================
void forwCB(Fl_Widget*, TrainWindow* tw)
{
	tw->advanceTrain(2);
	tw->damageMe();
}
//***************************************************************************
//
// * Reverse the movement of the train
//===========================================================================
void backCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	tw->advanceTrain(-2);
	tw->damageMe();
}


static unsigned long lastRedraw = 0;
static unsigned long buttonBuffer = 0;
static unsigned long buffer = 10;

static unsigned long invincibleStart = 0;
static unsigned long deadTimer = 0;
unsigned long reversiBuffer = 0;

//***************************************************************************
//
// * Callback for idling - if things are sitting, this gets called
// if the run button is pushed, then we need to make the train go.
// This is taken from the old "RunButton" demo.
// another nice problem to have - most likely, we'll be too fast
// don't draw more than 30 times per second
//===========================================================================
void runButtonCB(TrainWindow* tw)
//===========================================================================
{
	if (tw->trainView->game_state == CLOBBY) {
		if (tw->cp5Button->value()) tw->startingChapter = 4;
		else if (tw->cp4Button->value()) tw->startingChapter = 3;
		else if (tw->cp3Button->value()) tw->startingChapter = 2;
		else if (tw->cp2Button->value()) tw->startingChapter = 1;
		else if (tw->cp1Button->value()) tw->startingChapter = 0;
		else tw->startingChapter = -1;
		if (tw->startingChapter != -1) {
			tw->trainView->chapter = tw->startingChapter;
			tw->trainView->switchChapter(tw->trainView->chapter);
		}
		else {
			tw->trainView->chapter = 0;
			tw->trainView->switchChapter(0);
		}
		switch (tw->startingChapter) {
		case -1:
			break;
		case 0:
			tw->m_Track.first_P2 = false;
			tw->m_Track.first_P5 = false;
			break;
		case 1:
			break;
		case 2:
			tw->m_Track.first_P2 = false;
			break;
		case 3:
			tw->m_Track.first_P2 = false;
			break;
		case 4:
			tw->m_Track.first_P2 = false;
			break;
		default:
			tw->trainView->chapter = 0;
		}
	}

	//use spacebar to pause
	if (buttonBuffer > 0) buttonBuffer--;
	if (buttonBuffer == 0) {
		if (GetKeyState(VK_SPACE) < 0) {
			tw->runButton->value(!tw->runButton->value());
		}
	}

	if (tw->runButton->value()) {	// only advance time if appropriate

		//put away boss
		if (tw->trainView->chapter != 1 && tw->m_Track.miniBoss) tw->m_Track.miniBoss = false;
		if (tw->trainView->chapter != 4 && tw->m_Track.mainBoss) tw->m_Track.mainBoss = false;
		if (tw->trainView->chapter != 5 && tw->m_Track.extraBoss) tw->m_Track.extraBoss = false;

		if (clock() - lastRedraw > CLOCKS_PER_SEC / 30) {
			if (clock() - invincibleStart > CLOCKS_PER_SEC) tw->m_Track.player.invincible = false;

			//button input
			if (buttonBuffer == 0) {
				if (GetKeyState(VK_SPACE)) {
					tw->runButton->value(!tw->runButton->value());
					buttonBuffer = buffer;
				}
				if (GetAsyncKeyState('A') && tw->m_Track.lane > -1 && tw->speed->value() > 0.1f) {
					tw->m_Track.lane--;
					buttonBuffer = buffer;
				}
				if (GetAsyncKeyState('D') && tw->m_Track.lane < 1 && tw->speed->value() > 0.1f) {
					tw->m_Track.lane++;
					buttonBuffer = buffer;
				}
			}
			if (GetAsyncKeyState('W') && tw->m_Track.jumpingState == -1 && tw->speed->value() > 0.1f) {
				tw->m_Track.jumpingState = 0;
			}
			
			if (tw->trainView->game_state == CGAME)
			{
				//open the door
				if (tw->trainView->camera_movement_state == 0 && tw->trainView->door_offset > -0.5) tw->trainView->door_offset -= 0.02f; //open the door in the begin

				//player clipping collision
				if (tw->m_Track.miniBoss && abs(tw->m_Track.switchLane - MiniBoss::bossLane) < 0.1 && abs(MiniBoss::bossLane - MiniBoss::bossTarget) < 0.05) {
					//endReset(tw);
				}
				//clipping spawn
				if (tw->m_Track.miniBoss && MiniBoss::bossLane <= 1.2f) {
					tw->m_Track.obstacles.push_back(Obstacle(tw->m_Track.trainU + 0.4, (int)MiniBoss::bossTarget, 0));
				}

				//player multiball collision
				if (MainBoss::multiBallForward < 0.01 
					&& abs(((MainBoss::multiBallUp / 5.0f) - 1.0f) - ((tw->m_Track.jumpingState == -1) ? 0.0f : tw->m_Track.airbornePosition[tw->m_Track.jumpingState])) < 0.4f 
					&& abs(MainBoss::multiBallCross - tw->m_Track.switchLane) < 0.4f && !tw->debug_mode->value()) {
					endReset(tw);
				}

				//miniBoss movement
				if (tw->m_Track.miniBoss) {
					if (MiniBoss::bossTarget < MiniBoss::bossLane) MiniBoss::bossLane -= 0.05;
					if (MiniBoss::bossTarget > MiniBoss::bossLane) MiniBoss::bossLane += 0.05;
				}

				//mainBoss projectile movement
				float difference = 0.02;
				if (tw->m_Track.mainBoss) {
					if (abs(MainBoss::multiBallForward - MainBoss::targetForward) < difference * 0.15f
						&& abs(MainBoss::multiBallUp - MainBoss::targetUp) < difference * 5.0f
						&& abs(MainBoss::multiBallCross - MainBoss::targetCross) < difference) {
						MainBoss::targetForward = (float)(rand() % 2) * 0.15f;
						MainBoss::targetUp = (float)(rand() % 2) * 5.0f + 5.0f;
						MainBoss::targetCross = (float)((rand() % 3) - 1);
					}
					if (MainBoss::multiBallForward > MainBoss::targetForward) MainBoss::multiBallForward -= difference * 0.15f;
					else MainBoss::multiBallForward += difference * 0.15f;
					if (MainBoss::multiBallUp > MainBoss::targetUp) MainBoss::multiBallUp -= difference * 5.0f;
					else MainBoss::multiBallUp += difference * 5.0f;
					if (MainBoss::multiBallCross > MainBoss::targetCross) MainBoss::multiBallCross -= difference;
					else MainBoss::multiBallCross += difference;
				}

				//reversi spawn
				if (tw->m_Track.extraBoss) {
					if (reversiBuffer == 0) reversiBuffer = clock();
					if (clock() - reversiBuffer > CLOCKS_PER_SEC * 5) {

						//reset
						tw->m_Track.obstacles = {};
						tw->m_Track.wall = {};
						tw->m_Track.wallLocation = { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} };

						//poke holes
						int first_hole = rand() % 6;
						int second_hole = rand() % 6;
						while (second_hole == first_hole) second_hole = rand() % 6;
						for (int row = 0; row < 4; row++) {
							for (int col = 0; col < 5; col++) {
								if (!((row - 1 == first_hole / 3 && col - 1 == first_hole % 3) || (row - 1 == second_hole / 3 && col - 1 == second_hole % 3)) && !(rand() % 6 == 0)) {
									int color = rand() % 2;
									tw->m_Track.wall.push_back(Obstacle(tw->m_Track.trainU + 0.4, col - 2, row - 1, color));
									tw->m_Track.wallLocation[row][col] = color + 1;
								}
							}
						}

						//make obstacles real
						for (int obstacle = 0; obstacle < (int)tw->m_Track.wall.size(); obstacle++) {
							tw->m_Track.obstacles.push_back(tw->m_Track.wall[obstacle]);
						}
						reversiBuffer = clock();
					}
				}
				if (tw->trainView->chapter == 5) {
					//printf("%d 1\n", tw->m_Track.throwableObstacles.size());
					if ((int)tw->m_Track.obstacles.size() > 0 && abs(tw->m_Track.trainU - tw->m_Track.obstacles[0].position) < 0.01) {
						loadThrowableObstacles(tw, getPlayerReversiGridLocation(tw));
						while ((int)tw->m_Track.throwableObstacles.size() > 0) {
							tw->m_Track.throwingPosition.push_back(Obstacle(0.0f, tw->m_Track.throwableObstacles[0][1], tw->m_Track.throwableObstacles[0][0], 0));
							tw->m_Track.throwableObstacles.erase(tw->m_Track.throwableObstacles.begin());
						}
					}
					//printf("%d 2\n", tw->m_Track.throwableObstacles.size());
				}

				//player obstacle collision
				for (int obstacle = 0; obstacle < tw->m_Track.obstacles.size(); obstacle++) {
					if (tw->m_Track.collision(obstacle) && !tw->debug_mode->value()) {
						endReset(tw);
						break;
					}
				}

				//player money collection
				for (int money = 0; money < tw->m_Track.money.size(); money++) {
					if (tw->m_Track.collection(money)) {
						tw->m_Track.money.erase(tw->m_Track.money.begin() + money);
						tw->m_Track.money_collected += 1 * (tw->cudaButton->value() + 1);
						tw->m_Track.score += tw->speed->value();
						alSourcePlay(tw->trainView->moneySource);
						break;
					}
				}

				if (AL_SOURCE_STATE == 4116) alSourceStop(tw->trainView->moneySource);
				if (tw->trainView->door_offset < -0.25f) tw->m_Track.score += tw->speed->value() * (tw->shaderButton->value() + 1); // add score after door opened 50%
				if (tw->speed->value() < 4) tw->speed->value(tw->speed->value() + 0.0001);
				if (tw->trainView->door_offset < -0.25f) tw->advanceTrain(); // run out when door is opened 50%
			}
			tw->trainView->rotate_objects();
			tw->trainView->shootFireworks(); // after chapter 5
			if (tw->trainView->firework)
				for (int i = 0; i < tw->trainView->num_firework; i++)
					if (tw->trainView->firework[i]->isShoot())
						tw->trainView->firework[i]->advanceFirework();
			if (tw->trainView->rain && tw->trainView->chapter == 3)
				tw->trainView->rain->advanceRain();
			tw->trainView->filterAdvance();
			if (tw->trainView->star)
				tw->trainView->star->advanceStar();
			tw->trainView->mainBossAdvance();
			tw->trainView->extraBossHPAdvance();
			lastRedraw = clock();
			tw->damageMe();
		}
	}
}

//***************************************************************************
//
// * Load the control points from the files
//===========================================================================
void loadCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	const char* fname =
		fl_file_chooser("Pick a Path File", "*.txt", "../GraphicsRush/TrackFiles/default.txt");
	if (fname) {
		tw->m_Track.readPoints(fname);
		std::string file_name = tw->trainView->getFileName(fname);
		for (int find_chapter_path_file = 0; find_chapter_path_file < tw->trainView->chapter_path_file_name.size(); find_chapter_path_file++)
		{
			if (file_name == tw->trainView->chapter_path_file_name[find_chapter_path_file])
			{
				tw->trainView->chapter = find_chapter_path_file;
				tw->trainView->switchChapter(find_chapter_path_file);
				break;
			}
		}
		tw->damageMe();
	}
}
//***************************************************************************
//
// * Save the control points
//===========================================================================
void saveCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	const char* fname =
		fl_input("File name for save (should be *.txt)", "../GraphicsRush/TrackFiles/");
	if (fname)
		tw->m_Track.writePoints(fname);
}

//***************************************************************************
//
// * Rotate the selected control point about x axis
//===========================================================================
void rollx(TrainWindow* tw, float dir)
{
	int s = tw->trainView->selectedCube;
	if (s >= 0) {
		Pnt3f old = tw->m_Track.points[s].orient;
		float si = sin(((float)M_PI_4) * dir);
		float co = cos(((float)M_PI_4) * dir);
		tw->m_Track.points[s].orient.y = co * old.y - si * old.z;
		tw->m_Track.points[s].orient.z = si * old.y + co * old.z;
	}
	tw->damageMe();
}

//***************************************************************************
//
// * Rotate the selected control point about x axis by one more degree
//===========================================================================
void rpxCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	rollx(tw, 1);
}
//***************************************************************************
//
// * Rotate the selected control point  about x axis by less one degree
//===========================================================================
void rmxCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	rollx(tw, -1);
}

//***************************************************************************
//
// * Rotate the selected control point  about z axis
//===========================================================================
void rollz(TrainWindow* tw, float dir)
//===========================================================================
{
	int s = tw->trainView->selectedCube;
	if (s >= 0) {

		Pnt3f old = tw->m_Track.points[s].orient;

		float si = sin(((float)M_PI_4) * dir);
		float co = cos(((float)M_PI_4) * dir);

		tw->m_Track.points[s].orient.y = co * old.y - si * old.x;
		tw->m_Track.points[s].orient.x = si * old.y + co * old.x;
	}

	tw->damageMe();
}

//***************************************************************************
//
// * Rotate the selected control point  about the z axis one more degree
//===========================================================================
void rpzCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	rollz(tw, 1);
}

//***************************************************************************
//
// *  Rotate the selected control point  about the z axis one less degree
//===========================================================================
void rmzCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	rollz(tw, -1);
}

void endReset(TrainWindow* tw) {
	if (tw->m_Track.player.invincible) return;
	if (tw->thighButton->value() && tw->m_Track.player.items[THIGH] > 0) {
		tw->m_Track.player.items[THIGH]--;
		tw->thighButton->value(0);
		tw->m_Track.player.invincible = true;
		invincibleStart = clock();
		return;
	}

	tw->m_Track.throwingPosition = {};

	if (tw->trainView->game_state == CGAME && deadTimer == 0)
	{
		deadTimer = clock();
		tw->trainView->game_state = CDEAD; //***need to design***
	}

	if (tw->speed->value() > 0.01) tw->m_Track.saveSpeed = tw->speed->value();
	tw->speed->value(0);
}

int getPlayerReversiGridLocation(TrainWindow* tw) {
	int lane = 0, height = 0;
	if (tw->m_Track.switchLane < -0.33f) lane = -1;
	else if (tw->m_Track.switchLane > 0.33f) lane = 1;
	if (tw->m_Track.jumpingState > -1 && tw->m_Track.airbornePosition[tw->m_Track.jumpingState] > 0.5) height = 1;
	return (height + 1) * 5 + lane + 2;
};

void loadThrowableObstacles(TrainWindow* tw, int playerPosition) {
	int player_x = playerPosition % 5, player_y = playerPosition / 5;
	for (int row = -1; row < 2; row++) {
		for (int col = -1; col < 2; col++) {
			if (row == 0 && col == 0) col++;
			std::vector<std::vector<int>> throwable_result = {};
			reversiRecursion(player_x, player_y, row, col, tw, throwable_result);
		}
	}
};

void reversiRecursion(int player_x, int player_y, int row, int col, TrainWindow* tw, vector<vector<int>> &throwable_result) {

	//check if in range
	if (player_x + col < 0 || player_x + col > 4) return;
	if (player_y + row < 0 || player_y + row > 3) return;

	if (tw->m_Track.wallLocation[player_y + row][player_x + col] == 2) {
		throwable_result.push_back({ player_y + row, player_x + col });
		reversiRecursion(player_x + col, player_y + row, row, col, tw, throwable_result);
	}
	else if (tw->m_Track.wallLocation[player_y + row][player_x + col] == 1) {
		for (int hole = 0; hole < (int)throwable_result.size(); hole++) {
			tw->m_Track.wallLocation[throwable_result[hole][0]][throwable_result[hole][1]] = 1;
			tw->m_Track.throwableObstacles.push_back(throwable_result[hole]);
		}
	}
};

void resetReversiBoss(TrainWindow* tw) {
	ExtraBoss::health = 1;
	ExtraBoss::health_minus = 1;
	tw->m_Track.throwableObstacles = {};
	tw->m_Track.throwingPosition = {};
	reversiBuffer = 0;
};

void endResetForCallBack(TrainWindow* tw) {
	//save score
	if (tw->m_Track.score > tw->m_Track.player.highscore) tw->m_Track.player.highscore = tw->m_Track.score;
	tw->m_Track.player.money_total += tw->m_Track.money_collected;
	tw->m_Track.player.saveFile();

	//reset variables
	tw->m_Track.trainU = 0.0f;
	tw->m_Track.lane = 0;
	tw->m_Track.switchLane = 0.0f;
	tw->m_Track.jumpingState = -1;

	//miniboss
	MiniBoss::bossLane = 5;
	MiniBoss::clipping = -99;

	//mainboss
	MainBoss::multiBallForward = 0.4f;

	//extraBoss
	resetReversiBoss(tw);

	tw->trainView->camera_movement_state = 0;
	tw->trainView->camera_movement_index = 0;
	tw->trainView->door_offset = 0.0f;
	tw->speed->value(tw->m_Track.saveSpeed);
	if (tw->debug_mode->value()) {
		tw->runButton->value(0);
		tw->trainView->game_state = CGAME;
	}
	else if (verState == 1 && tw->ver2Button->value() && tw->m_Track.player.items[VER2] > 0) {
		tw->m_Track.player.items[VER2]--;
		tw->ver2Button->value(0);
		verState = 2;
		tw->trainView->game_state = CGAME;
	}
	else if (verState == 2 && tw->ver3Button->value() && tw->m_Track.player.items[VER3] > 0) {
		tw->m_Track.player.items[VER3]--;
		tw->ver3Button->value(0);
		verState = 3;
		tw->trainView->game_state = CGAME;
	}
	else
	{
		verState = 1;
		tw->m_Track.first_P2 = true;
		tw->m_Track.first_P5 = true;
		tw->m_Track.miniBoss = false;
		tw->m_Track.mainBoss = false;
		tw->m_Track.extraBoss = false;
		tw->m_Track.score = 0;
		tw->m_Track.money_collected = 0;
		tw->speed->value(tw->startingSpeed);
		tw->trainView->switchChapter(0);
		tw->trainView->game_state = CLOBBY;
		tw->startingChapter = -1;
		deadTimer = 0;
		tw->trainView->finish_computer_graphics = false;
	}
	tw->m_Track.obstacles = {};
	tw->m_Track.money = {};
}