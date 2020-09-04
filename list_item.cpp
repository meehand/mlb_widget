#include <GL/freeglut.h>
#include <ft2build.h>
#include <IL/il.h>
#include FT_FREETYPE_H

#include "list_item.h"

void draw_texture(float, float, float, string, Texture2D);
void draw_selection(float, float, float, float);
void draw_txt(float, float, string, void *);

/* this set_values() only needs to be set once, so we don't reload textures over and over */
void list_item::set_values(float set_x, float set_y, float set_width, string set_top_txt, string set_btm_txt, string set_image_filename)
{
	x = set_x;
	y = set_y;
	width = set_width;
	expnd_x = x - (width * 0.25f);
	expnd_y = y - (width * 0.25f);
	expnd_width = width * 1.5f;
	top_txt = set_top_txt;
	btm_txt = set_btm_txt;
	image_filename = set_image_filename;
	texture_obj.LoadTexture(image_filename.c_str());
}

/* set_values() to move a list_item */
void list_item::set_values(float set_x, float set_y, float set_width)
{
	x = set_x;
	y = set_y;
	width = set_width;
	expnd_x = x - (width * 0.25f);
	expnd_y = y - (width * 0.25f);
	expnd_width = width * 1.5f;
}

/* draw list_item */
void list_item::draw()
{
	if (selected)
		{
		draw_selection(expnd_x, expnd_y, expnd_width, 0.01f);
		draw_texture(expnd_x, expnd_y, expnd_width, image_filename, texture_obj);
		draw_txt(expnd_x - 0.02f, expnd_y + expnd_width + 0.03f, top_txt, GLUT_BITMAP_HELVETICA_18);
		draw_txt(expnd_x, expnd_y - 0.05f, btm_txt, GLUT_BITMAP_HELVETICA_12);
		}
	else
		{
		draw_texture(x, y, width, image_filename, texture_obj);
		}
}

void list_item::select
	(
	bool set
	)
{
selected = set;
}

void  list_item::remove_files()
{
	remove(image_filename.c_str());
}

void draw_texture(float x, float y, float width, string image_filename, Texture2D texture_obj)
	{
	texture_obj.Draw(x, y, width);
	}

void draw_selection(float x, float y, float width, float margin)
	{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(x - margin, y - margin);
	glVertex2f(x + width + margin, y - margin);
	glVertex2f(x + width + margin, y + width + margin);
	glVertex2f(x - margin, y + width + margin);
	glEnd();
	}

void draw_txt(float x, float y, string txt, void* font)
{
	glRasterPos2f(x, y );
	glColor3f(1.0f, 1.0f, 1.0f);
	const unsigned char* c_txt = reinterpret_cast<const unsigned char*>(txt.c_str());
	glutBitmapString(font, c_txt);
}


