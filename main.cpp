#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <ctime>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "list_item.h"
#include "list_item_grid.h"
#include "api_source.h"
#include "Texture2D.h"

using namespace std;

#define SCROLL_INTERVAL 0.4f
#define KEY_ESC		27

void init_gl(int argc, char** argv);
void display();
void idle();
void special_down_callback(int key, int x, int y);
void special_up_callback(int key, int x, int y);
void key_callback(unsigned char key, int x, int y);
bool timer_check(clock_t timer, float interval);
bool handle_pic_change();
bool handle_day_change();


bool right_press = false;
bool left_press = false;
bool up_press = false;
bool down_press = false;
clock_t right_timer = 0;
clock_t left_timer = 0;
list_item_grid* game_list;
Texture2D background;
api_source api;

/*GL initialization*/
void init_gl(int argc, char** argv)
{
	ilInit();								//Intialize DevIL
	iluInit();
	ilutInit();

	ilutRenderer(ILUT_OPENGL);
	glutInit(&argc, argv);                 // Initialize GLUT
	glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
	glutFullScreen();
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutIdleFunc(idle); // Register display callback handler for window re-paint
	glutSpecialFunc(special_down_callback);
	glutSpecialUpFunc(special_up_callback);
	glutKeyboardFunc(key_callback);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	background.LoadTexture("Resources/background.jpg");
}

/* GL display function*/
void display() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
   background.Draw(-1.0f, -1.0f, 2.0f);  
   game_list->draw();
   glFlush();  // Render now
}

/* GL idle function*/
void idle()
{
	bool change = false;
	change = handle_pic_change();
	change = change || handle_day_change();

	//Drawing
	if(change)
		display();
}

int main(int argc, char** argv) {
	time_t source_day = time(0);
	tm ltm;
	

	init_gl(argc, argv);

	localtime_s(&ltm, &source_day);
	ltm.tm_mday--; //use yesterday to give time to upload pics
	mktime(&ltm); //Nomralize time
	api.set_source_day(ltm);
	api.refresh_source();
	game_list = new list_item_grid(api.get_num_games(), -0.85f, 0.0f, 0.2f, 0.1f, &api);
	glutMainLoop();           // Enter the event-processing loop

	game_list->remove_files();
	delete game_list;
	
}

/* handle press down key */
void special_down_callback(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		right_press = true;
		break;

	case GLUT_KEY_LEFT:
		left_press = true;
		break;

	case GLUT_KEY_UP:
		up_press = true;
		break;

	case GLUT_KEY_DOWN:
		down_press = true;
		break;

	default:
		break;
	}
}

/* handle key release */
void special_up_callback(int key, int x, int y)
{
	switch (key)
	{
	 case GLUT_KEY_RIGHT:
		 right_press = false;
		 break;

	 case GLUT_KEY_LEFT:
		 left_press = false;
		 break;

	 default:
		 break;
	}
}

/* handle escape key */
void key_callback(unsigned char key, int x, int y)
{
	switch(key)
	{
	case KEY_ESC:
		glutFullScreenToggle();
		break;

	default:
		break;
	}
}

/* timer check for press and hold of left/right arrow keys */
bool timer_check( clock_t timer, float interval )
{
	return(timer == 0 || (clock() - timer) > (long)(interval * CLOCKS_PER_SEC));
}

/* moving to new picture */
bool handle_pic_change()
{
	bool change = false;
	if (right_press && !left_press)
	{
		if (timer_check(right_timer, SCROLL_INTERVAL))
		{
			game_list->select_next();
			change = true;
			right_timer = clock() + (clock_t)(CLOCKS_PER_SEC * (right_timer == 0 ? SCROLL_INTERVAL : 0.0f));
		}
	}
	else if (left_press && !right_press)
	{
		if (timer_check(left_timer, SCROLL_INTERVAL))
		{
			game_list->select_prev();
			change = true;
			left_timer = clock() + (clock_t)(CLOCKS_PER_SEC * (left_timer == 0 ? SCROLL_INTERVAL : 0.0f));
		}
	}
	else
	{
		right_timer = 0;
		left_timer = 0;
	}
	return(change);
}

/* moving to new day */
bool handle_day_change()
{
	bool change = false;
	if(up_press)
	{
		api.move_source_day(true);
		change = true;
		up_press = false;
	}
	else if(down_press)
	{
		api.move_source_day(false);
		change = true;
		down_press = false;
	}

	if (change)
	{
		api.refresh_source();
		*game_list = list_item_grid(api.get_num_games(), -0.85f, 0.0f, 0.2f, 0.1f, &api);
	}
		

	return(change);
}