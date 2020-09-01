#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctime>

#include "square.h"
#include "square_grid.h"
#include "api_source.h"

using namespace std;

#define SCROLL_INTERVAL 0.4f

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool timer_check(clock_t timer, float interval);

bool right_press = false;
bool left_press = false;


int main(void) {
	clock_t right_timer = 0;
	clock_t left_timer = 0;
	api_source api_source;
	time_t source_day = time(0);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "OpenGL Example", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	api_source.set_source_day(source_day);
	api_source.refresh_source();
	square_grid* game_list = new square_grid(api_source.get_num_games(), -0.85f, 0.0f, 0.2f, 0.1f);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)) {

		//Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		if( right_press && !left_press )
			{
			if( timer_check(right_timer, SCROLL_INTERVAL) )
				{
				game_list->select_next();
				right_timer = clock() + (clock_t)(CLOCKS_PER_SEC * (right_timer == 0 ? SCROLL_INTERVAL : 0.0f) );
				}	
			}
		else if( left_press && !right_press)
			{
			if (timer_check(left_timer, SCROLL_INTERVAL))
				{
				game_list->select_prev();
				left_timer = clock() + (clock_t)(CLOCKS_PER_SEC * (left_timer == 0 ? SCROLL_INTERVAL : 0.0f));
				}
			}
		else
			{
			right_timer = 0;
			left_timer = 0;
			}
			

		//Drawing
		game_list->draw();

		//Swap buffer and check for events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete game_list;
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Possible improvement: handle press and hold
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
		right_press = true;
		}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		{
		right_press = false;
		}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
		left_press = true;
		}	
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		{
		left_press = false;
		}
		
}

bool timer_check( clock_t timer, float interval )
{
	return(timer == 0 || (clock() - timer) > (long)(interval * CLOCKS_PER_SEC));
}