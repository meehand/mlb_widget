#pragma once
#include <string>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "Texture2D.h"

using namespace std;

class list_item
{
	private:
		float x, y, width;
		float expnd_x, expnd_y, expnd_width;
		int game_num;
		bool selected;
		string top_txt, btm_txt, image_filename;
		Texture2D texture_obj;

	public:
		list_item()
			{
			x = 0.0f;
			y = 0.0f;
			width = 0.0f;
			expnd_x = 0.0f;
			expnd_y = 0.0f;
			expnd_width = 0.0f;
			selected = false;
			top_txt = "";
			btm_txt = "";
			}
		void set_values(float, float, float, string, string, string);
		void set_values(float, float, float);
		void draw();
		void select(bool);
		void remove_files();

};

