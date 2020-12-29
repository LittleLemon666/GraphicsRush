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

#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"
#include "Boss.H"

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
	if (tw->runButton->value()) {	// only advance time if appropriate
		if (clock() - lastRedraw > CLOCKS_PER_SEC / 30) {
			

			//button input
			if (buttonBuffer > 0) buttonBuffer--;
			if (buttonBuffer == 0) {
				if (GetAsyncKeyState('A') && tw->m_Track.lane > -1) {
					tw->m_Track.lane--;
					buttonBuffer = buffer;
				}
				if (GetAsyncKeyState('D') && tw->m_Track.lane < 1) {
					tw->m_Track.lane++;
					buttonBuffer = buffer;
				}
				if (GetAsyncKeyState('W') && tw->m_Track.jumpingState == -1) {
					tw->m_Track.jumpingState = 0;
					buttonBuffer = buffer;
				}
			}
			
			if (tw->trainView->game_state == CGAME)
			{
				//open the door
				if (tw->trainView->camera_movement_state == 0 && tw->trainView->door_offset > -0.5) tw->trainView->door_offset -= 0.02f; //open the door in the begin

				//player clipping collision
				if (tw->m_Track.miniBoss && abs(tw->m_Track.switchLane - MiniBoss::bossLane) < 0.1 && abs(MiniBoss::bossLane - MiniBoss::bossTarget) < 0.05) {
					Sleep(1000);
					tw->runButton->value(0);
					tw->speed->value(1);
					tw->m_Track.trainU = 0.0f;
					tw->m_Track.lane = 0;
					tw->m_Track.switchLane = 0.0f;
					tw->m_Track.jumpingState = -1;
					MiniBoss::bossLane = 5;
					MiniBoss::clipping = -99;
					tw->trainView->camera_movement_state = 0;
					tw->trainView->camera_movement_index = 0;
					tw->trainView->door_offset = 0.0f;

					if (!tw->debug_mode->value())
					{
						tw->trainView->switchChapter(0);
					}
				}

				//player multiball collision
				if (MainBoss::multiBallForward < 0.01 
					&& abs(((MainBoss::multiBallUp / 5.0f) - 1.0f) - ((tw->m_Track.jumpingState == -1) ? 0.0f : tw->m_Track.airbornePosition[tw->m_Track.jumpingState])) < 0.4f 
					&& abs(MainBoss::multiBallCross - tw->m_Track.switchLane) < 0.4f) {
					Sleep(1000);
					tw->runButton->value(0);
					tw->speed->value(1);
					tw->m_Track.trainU = 0.0f;
					tw->m_Track.lane = 0;
					tw->m_Track.switchLane = 0.0f;
					tw->m_Track.jumpingState = -1;
					MainBoss::multiBallForward = 0.4f;
					tw->trainView->camera_movement_state = 0;
					tw->trainView->camera_movement_index = 0;
					tw->trainView->door_offset = 0.0f;

					if (!tw->debug_mode->value())
					{
						tw->trainView->switchChapter(0);
					}
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

				//player obstacle collision
				for (int obstacle = 0; obstacle < tw->m_Track.obstacles.size(); obstacle++) {
					if (tw->m_Track.collision(obstacle)) {
						Sleep(1000);
						tw->runButton->value(0);
						tw->speed->value(1);
						tw->m_Track.trainU = 0.0f;
						tw->m_Track.lane = 0;
						tw->m_Track.switchLane = 0.0f;
						tw->m_Track.jumpingState = -1;
						tw->trainView->camera_movement_state = 0;
						tw->trainView->camera_movement_index = 0;
						tw->trainView->door_offset = 0.0f;

						if (!tw->debug_mode->value())
						{
							tw->trainView->switchChapter(0);
						}
						break;
					}
				}

				//player money collection
				for (int money = 0; money < tw->m_Track.money.size(); money++) {
					if (tw->m_Track.collection(money)) {
						tw->m_Track.money.erase(tw->m_Track.money.begin() + money);
						tw->m_Track.money_collected++;
						tw->m_Track.score += tw->speed->value();
						alSourcePlay(tw->trainView->moneySource);
						break;
					}
				}

				if (AL_SOURCE_STATE == 4116) alSourceStop(tw->trainView->moneySource);
				if (tw->trainView->door_offset < -0.25f) tw->m_Track.score += tw->speed->value(); // add score after door opened 50%
				if (tw->speed->value() < 4) tw->speed->value(tw->speed->value() + 0.0001);
				if (tw->trainView->door_offset < -0.25f) tw->advanceTrain(); // run out when door is opened 50%
			}
			tw->trainView->money_rotate += 0.1f;
			if (tw->trainView->money_rotate > 360) tw->trainView->money_rotate -= 360;
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
