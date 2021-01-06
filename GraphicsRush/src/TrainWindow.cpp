/************************************************************************
     File:        TrainWindow.H

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu

     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						this class defines the window in which the project 
						runs - its the outer windows that contain all of 
						the widgets, including the "TrainView" which has the 
						actual OpenGL window in which the train is drawn

						You might want to modify this class to add new widgets
						for controlling	your train

						This takes care of lots of things - including installing 
						itself into the FlTk "idle" loop so that we get periodic 
						updates (if we're running the train).


     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/

#include <FL/fl.h>
#include <FL/Fl_Box.h>

// for using the real time clock
#include <time.h>

#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"

#include "Object.h"

//************************************************************************
//
// * Constructor
//========================================================================
TrainWindow::
TrainWindow(const int x, const int y) 
	: Fl_Double_Window(x,y,800,600,"Train and Roller Coaster")
//========================================================================
{
	// make all of the widgets
	begin();	// add to this widget
	{
		int pty=5;			// where the last widgets were drawn

		trainView = new TrainView(5,5,590,590);
		trainView->tw = this;
		trainView->m_pTrack = &m_Track;

		/*********************NEW ADDITIONS*********************/
		trainView->gmt.gmt_track = &m_Track;

		this->resizable(trainView);

		// to make resizing work better, put all the widgets in a group
		widgets = new Fl_Group(600,5,190,590);
		widgets->begin();

		runButton = new Fl_Button(605,pty,60,20,"Run");
		togglify(runButton, 1);

		Fl_Button* fb = new Fl_Button(700,pty,25,20,"@>>");
		fb->callback((Fl_Callback*)forwCB,this);
		Fl_Button* rb = new Fl_Button(670,pty,25,20,"@<<");
		rb->callback((Fl_Callback*)backCB,this);
		
		debug_mode = new Fl_Button(730,pty,65,20,"Debug");
		togglify(debug_mode,1);
  
		pty+=25;
		speed = new Fl_Value_Slider(655,pty,140,20,"speed");
		speed->range(0,10);
		speed->value(startingSpeed);
		speed->align(FL_ALIGN_LEFT);
		speed->type(FL_HORIZONTAL);

		pty += 30;

		// camera buttons - in a radio button group
		Fl_Group* camGroup = new Fl_Group(600,pty,195,20);
		camGroup->begin();
		worldCam = new Fl_Button(605, pty, 60, 20, "World");
        worldCam->type(FL_RADIO_BUTTON);		// radio button
        worldCam->value(0);			// turned on
        worldCam->selection_color((Fl_Color)3); // yellow when pressed
		worldCam->callback((Fl_Callback*)damageCB,this);
		trainCam = new Fl_Button(670, pty, 60, 20, "Train");
        trainCam->type(FL_RADIO_BUTTON);
        trainCam->value(1);
        trainCam->selection_color((Fl_Color)3);
		trainCam->callback((Fl_Callback*)damageCB,this);
		topCam = new Fl_Button(735, pty, 60, 20, "Top");
        topCam->type(FL_RADIO_BUTTON);
        topCam->value(0);
        topCam->selection_color((Fl_Color)3);
		topCam->callback((Fl_Callback*)damageCB,this);
		camGroup->end();

		pty += 30;

		// browser to select spline types
		// TODO: make sure these choices are the same as what the code supports
		splineBrowser = new Fl_Browser(605,pty,120,75,"Spline Type");
		splineBrowser->type(2);		// select
		splineBrowser->callback((Fl_Callback*)damageCB,this);
		splineBrowser->add("Linear");
		splineBrowser->add("Cardinal Cubic");
		splineBrowser->add("Cubic B-Spline");
		splineBrowser->select(2);

		pty += 110;

		// add and delete points
		Fl_Button* ap = new Fl_Button(605,pty,80,20,"Add Point");
		ap->callback((Fl_Callback*)addPointCB,this);
		Fl_Button* dp = new Fl_Button(690,pty,80,20,"Delete Point");
		dp->callback((Fl_Callback*)deletePointCB,this);

		pty += 25;
		// reset the points
		resetButton = new Fl_Button(735,pty,60,20,"Reset");
		resetButton->callback((Fl_Callback*)resetCB,this);
		Fl_Button* loadb = new Fl_Button(605,pty,60,20,"Load");
		loadb->callback((Fl_Callback*) loadCB, this);
		Fl_Button* saveb = new Fl_Button(670,pty,60,20,"Save");
		saveb->callback((Fl_Callback*) saveCB, this);

		pty += 25;
		// roll the points
		Fl_Button* rx = new Fl_Button(605,pty,30,20,"R+X");
		rx->callback((Fl_Callback*)rpxCB,this);
		Fl_Button* rxp = new Fl_Button(635,pty,30,20,"R-X");
		rxp->callback((Fl_Callback*)rmxCB,this);
		Fl_Button* rz = new Fl_Button(670,pty,30,20,"R+Z");
		rz->callback((Fl_Callback*)rpzCB,this);
		Fl_Button* rzp = new Fl_Button(700,pty,30,20,"R-Z");
		rzp->callback((Fl_Callback*)rmzCB,this);

		pty+=30;

		// TODO: add widgets for all of your fancier features here

		//�j�L (�i�I���@��)
		thighButton = new Fl_Button(605, pty, 60, 20, "Thigh");
		togglify(thighButton, 0);
		//revive once
		ver2Button = new Fl_Button(670, pty, 60, 20, "Ver2");
		togglify(ver2Button, 0);
		//revive second time
		ver3Button = new Fl_Button(735, pty, 60, 20, "Ver3");
		togglify(ver3Button, 0);

		pty += 30;

		//
		shaderButton = new Fl_Button(605, pty, 60, 20, "Shader");
		togglify(shaderButton, 0);
		//
		cudaButton = new Fl_Button(670, pty, 60, 20, "Cuda");
		togglify(cudaButton, 0);

		pty += 30;
		//checkpoint1
		cp1Button = new Fl_Button(605, pty, 60, 20, "CP1");
		togglify(cp1Button, 0);
		//checkpoint2
		cp2Button = new Fl_Button(670, pty, 60, 20, "CP2");
		togglify(cp2Button, 0);
		//checkpoint3
		cp3Button = new Fl_Button(735, pty, 60, 20, "CP3");
		togglify(cp3Button, 0);
		pty += 30;
		//checkpoint4
		cp4Button = new Fl_Button(605, pty, 60, 20, "CP4");
		togglify(cp4Button, 0);
		//checkpoint5
		cp5Button = new Fl_Button(670, pty, 60, 20, "CP5");
		togglify(cp5Button, 0);

		pty += 30;
		subdivisionButton = new Fl_Button(605, pty, 80, 20, "subdivision");
		togglify(subdivisionButton, 1);
		pty += 30;
#ifdef EXAMPLE_SOLUTION
		makeExampleWidgets(this,pty);
#endif

		// we need to make a little phantom widget to have things resize correctly
		Fl_Box* resizebox = new Fl_Box(600,595,200,5);
		widgets->resizable(resizebox);

		widgets->end();
	}
	end();	// done adding to this widget

	// set up callback on idle
	Fl::add_idle((void (*)(void*))runButtonCB,this);
}

//************************************************************************
//
// * handy utility to make a button into a toggle
//========================================================================
void TrainWindow::
togglify(Fl_Button* b, int val)
//========================================================================
{
	b->type(FL_TOGGLE_BUTTON);		// toggle
	b->value(val);		// turned off
	b->selection_color((Fl_Color)3); // yellow when pressed	
	b->callback((Fl_Callback*)damageCB,this);
}

//************************************************************************
//
// *
//========================================================================
void TrainWindow::
damageMe()
//========================================================================
{
	if (trainView->selectedCube >= ((int)m_Track.points.size()))
		trainView->selectedCube = 0;
	trainView->damage(1);
}

//************************************************************************
//
// * This will get called (approximately) 30 times per second
//   if the run button is pressed
//========================================================================
void TrainWindow::
advanceTrain(float dir)
//========================================================================
{
	float distance = 1.0f * speed->value(), walked = 0.0f;
	while (walked < distance) {
		trainView->gmt.setG_pos((int)m_Track.trainU);
		float ratio = m_Track.trainU - (int)m_Track.trainU;
		vec3 thisPos = trainView->gmt.calculate(ratio);
		vec3 nextPos = trainView->gmt.calculate(ratio + 1.0f / (float)trainView->PATH_DIVIDE);
		float difference = sqrt(pow(thisPos.x - nextPos.x, 2) + pow(thisPos.y - nextPos.y, 2) + pow(thisPos.z - nextPos.z, 2));
		if (walked + difference >= distance) {
			if (!trainView->pizza_time) m_Track.trainU += (1.0f / (float)trainView->PATH_DIVIDE) * ((distance - walked) / difference);
		}
		else if (!trainView->pizza_time) m_Track.trainU += 1.0f / (float)trainView->PATH_DIVIDE;
		walked += difference;
		//infinite reversi
		if (trainView->chapter == 5) {
			if (m_Track.trainU < 1 && trainView->screen_brightness < 1.0f) trainView->screen_brightness += 0.1f;
			if (ExtraBoss::health <= 0) {
				if (trainView->screen_brightness < 0.1f) {
					trainView->switchChapter(0);
					m_Track.trainU = 0.0f;
				}
				trainView->screen_brightness -= 0.1f;
			}
		}
		else {
			if (m_Track.trainU >= m_Track.points.size() - 0.15f) trainView->screen_brightness -= 0.1f;
			if (m_Track.trainU < 1 && trainView->screen_brightness < 1.0f) trainView->screen_brightness += 0.1f;
			if ((int)m_Track.trainU >= (int)m_Track.points.size())
			{
				if (trainView->chapter == 1) m_Track.first_P2 = false;
				if (trainView->chapter == 3) trainView->rain->rainEnd();
				if (trainView->chapter == 4)
				{
					m_Track.first_P5 = false;
					trainView->finishComputerGraphics();
				}
				m_Track.trainU -= (int)m_Track.points.size();
				trainView->switchChapter(trainView->chapter + 1);
				if (trainView->chapter == 3) trainView->rain->rainBegin(trainView->player_pos);
			}
		}
	}
	//#####################################################################
	// TODO: make this work for your train
	//#####################################################################
#ifdef EXAMPLE_SOLUTION
	// note - we give a little bit more example code here than normal,
	// so you can see how this works

	if (arcLength->value()) {
		float vel = ew.physics->value() ? physicsSpeed(this) : dir * (float)speed->value();
		world.trainU += arclenVtoV(world.trainU, vel, this);
	} else {
		world.trainU +=  dir * ((float)speed->value() * .1f);
	}

	float nct = static_cast<float>(world.points.size());
	if (world.trainU > nct) world.trainU -= nct;
	if (world.trainU < 0) world.trainU += nct;
#endif
}