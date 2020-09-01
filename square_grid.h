#include "square.h"
#include "api_source.h"
#include <GLFW\glfw3.h>

class square_grid
{
	private:
		square *squares;
		int num_squares;
		int selected;
		float init_x, y, width, margin, tot_width, onscreen_cnt;
		api_source *api;

	public:
		square_grid
			(int num_squares,
				float set_init_x,
				float set_y,
				float set_width,
				float set_margin,
				api_source set_api
			);

		~square_grid()
			{
			delete(squares);
			}

		void draw();
		void set_selected(int);
		void select_next();
		void select_prev();
		void init_squares(int);

		bool shift_squares_check(bool);
		void shift_squares(float);
};

