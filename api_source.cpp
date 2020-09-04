#include "api_source.h"
#include <stdio.h>

#include "curlpp/curl_easy.h"
#include "curlpp/curl_form.h"
#include "curlpp/curl_ios.h"
#include "curlpp/curl_exception.h"

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"


void break_up_str(int char_num, string* str);
void get_str_value_by_ptr(Document*, stringstream*, string *);
void get_bool_value_by_ptr(Document*, stringstream*, bool *);
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);

api_source::api_source()
{
	num_games = 0;
	url = "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2019-06-11&sportId=1";
}

int api_source::get_num_games()
{
	return num_games;
}

/* set source day */
void api_source::set_source_day(tm ltm)
{
	string append_url = "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=";
	stringstream temp_url;
	
	temp_url << append_url << ltm.tm_year + 1900 << "-" << ltm.tm_mon + 1 << "-" << ltm.tm_mday << "&sportId=1";
	url = temp_url.str();
	day = ltm;

}

/* moving to new source day */
void api_source::move_source_day(bool forward)
{
	string append_url = "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=";
	stringstream temp_url;
	if (forward)
	{
		day.tm_mday++;
		mktime(&day);
	}
	else
	{
		day.tm_mday--;
		mktime(&day);
	}

	temp_url << append_url << day.tm_year + 1900 << "-" << day.tm_mon + 1 << "-" << day.tm_mday << "&sportId=1";
	url = temp_url.str();
	

}

/* refresh api source */
void api_source::refresh_source()
{
	
	stringstream str;
	try
	{
		curl::curl_ios<std::stringstream> writer(str);
		curl::curl_easy easy(writer);

		easy.add<CURLOPT_URL>(url.data());
		easy.add<CURLOPT_FOLLOWLOCATION>(1L);

		easy.perform();
	}
	catch (curl::curl_easy_exception const& error)
	{
		auto errors = error.get_traceback();
		error.print_traceback();
	}
	document.Parse(str.str().c_str());
	assert(document.HasMember("totalGames"));
	assert(document["totalGames"].IsInt());
	num_games = document["totalGames"].GetInt();

}

string api_source::get_versus(int game)
{
	stringstream versus;
	string ret_str;
	string away, home;
	bool home_win;
	stringstream json_ptr;

	json_ptr << "/dates/0/games/" << game << "/teams/away/team/name";
	get_str_value_by_ptr(&document, &json_ptr, &away);
	json_ptr.str("");

	json_ptr << "/dates/0/games/" << game << "/teams/home/team/name";
	get_str_value_by_ptr(&document, &json_ptr, &home);
	json_ptr.str("");

	json_ptr << "/dates/0/games/" << game << "/teams/home/isWinner";
	get_bool_value_by_ptr(&document, &json_ptr, &home_win);
	json_ptr.str("");

	//TODO: Tie?
	if (home_win)
	{
		versus << home.c_str() << " defeat " << away.c_str();
	}
	else
	{
		versus << away.c_str() << " defeat " << home.c_str();
	}

	ret_str.assign(versus.str()); 
	return(ret_str);
}

string api_source::get_blurb(int game)
{
	string blurb;	
	stringstream json_ptr;
	json_ptr << "/dates/0/games/" << game << "/content/editorial/recap/mlb/blurb";
	get_str_value_by_ptr(&document, &json_ptr, &blurb);

	break_up_str(45, &blurb); //TODO: Break on Space

	return(blurb);
}

string api_source::create_image(int game)
{
	CURL* curl;
	FILE* fp;
	string image_url;
	stringstream json_ptr, filename;

	json_ptr << "/dates/0/games/" << game << "/content/editorial/recap/mlb/image/cuts/0/src";
	get_str_value_by_ptr(&document, &json_ptr, &image_url);

	filename << "";
	curl = curl_easy_init();
	if (curl) {
		filename << ".\\game" << game << ".jpg";
		fopen_s(&fp, filename.str().c_str(), "wb");
		if (fp)
		{
			curl_easy_setopt(curl, CURLOPT_URL, image_url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			fclose(fp);
		}
	}
	return(filename.str());
}

void get_str_value_by_ptr(Document * doc, stringstream * json_stream, string *out_str)
{
	string ret_str = "";
	Pointer p(json_stream->str().c_str());
	Value* value_obj = GetValueByPointer(*doc, p);
	if (value_obj)
		out_str->assign(value_obj->GetString());
}

void get_bool_value_by_ptr(Document* doc, stringstream* json_stream, bool *out_int)
{
	string ret_str = "";
	Pointer p(json_stream->str().c_str());
	Value* value_obj = GetValueByPointer(*doc, p);
	if (value_obj)
		*out_int = value_obj->GetBool();
}

/* break up string on spaces for aesthetics  */
//TODO: Clean up text
void break_up_str(int char_num, string *str)
{
	size_t i;
	size_t line_tracker = 0, space_tracker = 0;
	for (i = 0; i < str->size(); i++)
	{
		if (str->at(i) == '\n') line_tracker = 0;
		if (str->at(i) == ' ') space_tracker = i;
		if (line_tracker >= char_num)
		{
			str->insert(space_tracker + 1, "\n"); //break on space
			line_tracker = 0;
		}
		line_tracker++;
	}
}

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}