#include "square_grid.h"
#include "Square.h"
#include "linmath.h"

square_grid::square_grid
	(
	int set_num_squares,
	float set_init_x,
	float set_y,
	float set_width,
	float set_margin,
	api_source set_api
	)
{
num_squares = set_num_squares;
squares = new square[num_squares];
init_x = set_init_x;
y = set_y;
width = set_width;
margin = set_margin;
tot_width = width + margin;
onscreen_cnt = floor((2.0f - (init_x + 1.0f))/(tot_width));
api = &set_api;

this->init_squares(0);
	

squares[0].select(true);
selected = 0;
}

void square_grid::init_squares(int start)
{
int i;
for (i = 0; i < num_squares; i++)
	{
	squares[i].set_values(init_x + ((width + margin) * (i - start)), y, width,"", api->get_blurb(i));
	}
}
void square_grid::draw()
{
	int i;
	for (i = 0; i < num_squares; i++)
		{
		squares[i].draw();
		}
}

void square_grid::set_selected(int i)
{
	squares[i].select(true);
}

void square_grid::select_next()
{
	squares[selected].select(false);

	if (selected == num_squares - 1)
		{
		selected = 0;
		this->init_squares(0);
		}
	else
		{
		selected++;
		if(selected > (onscreen_cnt / 2) && (num_squares - selected + 1) > (onscreen_cnt / 2))
			{
			this->init_squares(selected - (onscreen_cnt / 2));
			}	
		}
	squares[selected].select(true);
}

void square_grid::select_prev()
{
	squares[selected].select(false);

	if (selected == 0)
		{
		selected = num_squares - 1;
		this->init_squares(num_squares - onscreen_cnt);
		}
	else
		{
		selected--;
		if(selected < num_squares - (onscreen_cnt / 2) && (selected + 1) > (onscreen_cnt / 2))
			{
			this->init_squares(selected - (onscreen_cnt / 2));
			}	
		}
	squares[selected].select(true);
}

bool square_grid::shift_squares_check(bool forward)
{
	int rem_squares;
	bool past_mid;
	
	
	if( forward )
		{
		rem_squares = num_squares - selected;
		past_mid = (init_x + (tot_width * selected) > 0.0f);
		}
	else
		{
		rem_squares = selected;
		past_mid = ( (-1.0f * init_x) - (tot_width * (num_squares - selected))) < 0.0f;
		}
	
	
	float tot_length_rem = rem_squares * tot_width;
	return(past_mid && tot_length_rem > 1.0f);
}

void square_grid::shift_squares(float shift)
	{
	int i;
	for(i=0; i < num_squares; i++)
		{
		squares[i].shift_square(shift);
		}
	}


