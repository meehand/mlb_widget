#pragma once
#include <ctime>
#include <stdlib.h>
#include <string>

#include "rapidjson/document.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

using namespace rapidjson;
using namespace std;

class api_source
{
private:
	int num_games;
	string url;
	Document document;
	tm day;

public:
	api_source();
	void set_source_day(tm);
	void move_source_day(bool);
	void refresh_source();
	int get_num_games();
	string get_versus(int);
	string get_blurb(int);
	string create_image(int game);
};

