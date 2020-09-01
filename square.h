#include<string>

using namespace std;

class square
{
	private:
		float x, y, width;
		float expnd_x, expnd_y, expnd_width;
		int game_num;
		bool selected;
		string top_txt, btm_txt;
		// image

	public:
		square()
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
		void set_values(float, float, float, string, string);
		void draw();
		void select(bool);
		void shift_square(float);
		

};

