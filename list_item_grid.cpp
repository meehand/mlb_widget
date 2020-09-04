#include "list_item_grid.h"
#include "list_item.h"
#include "linmath.h"

/* class for creating a grid of items starting at init_x. 
   each new item is width long with margin between			*/
list_item_grid::list_item_grid
	(
	int set_num_list_items,
	float set_init_x,
	float set_y,
	float set_width,
	float set_margin,
	api_source* set_api
	)
{
num_list_items = set_num_list_items;
list_items = new list_item[num_list_items];
init_x = set_init_x;
y = set_y;
width = set_width;
margin = set_margin;
tot_width = width + margin;
onscreen_cnt = (int)floor((2.0f - (init_x + 1.0f))/(tot_width));
api = set_api;
this->init_list_items(0);
list_items[0].select(true);
selected = 0;
}

void list_item_grid::init_list_items(int start)
{
int i;

for (i = 0; i < num_list_items; i++)
	{
	list_items[i].set_values(init_x + ((width + margin) * (i - start)), y, width, api->get_versus(i), api->get_blurb(i), api->create_image(i));
	}
}

void list_item_grid::shift_list_items(int start)
{
	int i;
	for (i = 0; i < num_list_items; i++)
	{
		list_items[i].set_values(init_x + ((width + margin) * (i - start)), y, width);
	}
}

void list_item_grid::draw()
{
	int i;
	for (i = 0; i < num_list_items; i++)
		{
		list_items[i].draw();
		}
}

void list_item_grid::set_selected(int i)
{
	list_items[i].select(true);
}

void list_item_grid::select_next()
{
	list_items[selected].select(false);

	if (selected == num_list_items - 1)
		{
		selected = 0;
		this->shift_list_items(0);
		}
	else
		{
		selected++;
		if(selected > (onscreen_cnt / 2) && (num_list_items - selected + 1) > (onscreen_cnt / 2))
			{
			this->shift_list_items(selected - (int)(onscreen_cnt / 2));
			}	
		}
	list_items[selected].select(true);
}

void list_item_grid::select_prev()
{
	list_items[selected].select(false);

	if (selected == 0)
		{
		selected = num_list_items - 1;
		this->shift_list_items(num_list_items - onscreen_cnt);
		}
	else
		{
		selected--;
		if(selected < num_list_items - (onscreen_cnt / 2) && (selected + 1) > (onscreen_cnt / 2))
			{
			this->shift_list_items(selected - (onscreen_cnt / 2));
			}	
		}
	list_items[selected].select(true);
}

/* class clean up function to remove associated texture files */
void list_item_grid::remove_files()
{
	int i;

	for (i = 0; i < num_list_items; i++)
	{
		list_items[i].remove_files();
	}
}
