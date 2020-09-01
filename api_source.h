#include "rapidjson/document.h"

#include <ctime>
#include <stdlib.h>
#include <string>

using namespace rapidjson;
using namespace std;

class api_source
{
private:
	int num_games;
	string url;
	Document document;

public:
	api_source();
	void set_source_day(time_t);
	void refresh_source();
	int get_num_games();
	string get_blurb(int);
};

