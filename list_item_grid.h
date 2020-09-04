#pragma once
#include "list_item.h"
#include "api_source.h"

class list_item_grid
{
	private:
		list_item* list_items;
		api_source* api;
		int num_list_items, selected, onscreen_cnt;
		float init_x, y, width, margin, tot_width;

	public:
		list_item_grid
			(int num_list_items,
				float set_init_x,
				float set_y,
				float set_width,
				float set_margin,
				api_source * set_api
			);

		void draw();
		void set_selected(int);
		void select_next();
		void select_prev();
		void init_list_items(int);
		void shift_list_items(int);
		void remove_files();
};

