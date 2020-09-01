#include <GLFW\glfw3.h>

#include "square.h"

void draw_inner(float x, float y, float width);
void draw_outer(float x, float y, float width, float margin);

void square::set_values(float set_x, float set_y, float set_width, string top_txt, string btm_txt)
{
	x = set_x;
	y = set_y;
	width = set_width;
	expnd_x = x - (width * 0.25f);
	expnd_y = y - (width * 0.25f);
	expnd_width = width * 1.5f;
}

void square::draw()
{
	if (selected)
		{
		draw_outer(expnd_x, expnd_y, expnd_width, 0.01f);
		draw_inner(expnd_x, expnd_y, expnd_width);
		}
	else
		{
		draw_inner(x, y, width);
		}
}

void square::select
	(
	bool set
	)
{
selected = set;
}

void square::shift_square(float shift)
	{
	x = x + shift;
	expnd_x = expnd_x + shift;
	}

void draw_inner(float x, float y, float width)
	{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + width);
	glVertex2f(x, y + width);
	glEnd();
	}

void draw_outer(float x, float y, float width, float margin)
	{
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(x - margin, y - margin);
	glVertex2f(x + width + margin, y - margin);
	glVertex2f(x + width + margin, y + width + margin);
	glVertex2f(x - margin, y + width + margin);
	glEnd();
	}


