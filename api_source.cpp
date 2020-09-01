#include "api_source.h"

#include "curlpp/curl_easy.h"
#include "curlpp/curl_form.h"
#include "curlpp/curl_ios.h"
#include "curlpp/curl_exception.h"

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"


api_source::api_source()
{
	num_games = 0;
	url = "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2019-06-11&sportId=1";
}

int api_source::get_num_games()
{
	return num_games;
}

void api_source::set_source_day(time_t time)
{
	string append_url = "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=";
	stringstream temp_url;
	tm ltm;
	localtime_s(&ltm, &time);
	temp_url << append_url << ltm.tm_year + 1900 << "-" << ltm.tm_mon + 1 << "-" << ltm.tm_mday << "&sportId=1";
	url.assign(temp_url.str());

}

void api_source::refresh_source()
{
	Document document;
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

string api_source::get_blurb(int game)
{
	string blurb;
	if (Value* blurb_obj = GetValueByPointer(document, "/dates/0/games/0/content/editorial/recap/mlb/blurb"))
		blurb = blurb_obj->GetString();

	return(blurb);
}
