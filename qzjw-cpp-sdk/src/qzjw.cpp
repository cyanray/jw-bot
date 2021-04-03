#include "qzjw.h"
#include <sstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <CURLWrapper.h>
#include <regex>
#include <iostream>
#include <../../HTMLReader/include/HTMLReader.h>
using namespace Cyan;
using namespace std;
using namespace nlohmann;

namespace cyanray
{
	bool Jw::Login(const string& uid, const string& pwd)
	{
		stringstream url;
		url << api_prefix_
			<< "/app.do?method=authUser&xh="
			<< uid
			<< "&pwd="
			<< pwd;
		HTTP http;
		auto resp = http.Get(url.str());
		if (!resp.Ready) throw runtime_error("请求无响应.");
		if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");
		json re_json = json::parse(resp.Content);
		string flag = re_json["flag"].get<string>();
		if (flag != "1")
		{
			string err_msg = re_json["msg"].get<string>();
		}
		token_ = re_json["token"].get<string>();
		return true;
	}

	vector<Jw::Course> Jw::GetCourses(const string& uid, int week, const string& semester)
	{
		if (token_.empty()) throw runtime_error("尚未登录.");
		stringstream url;
		url << api_prefix_
			<< "/app.do?method=getKbcxAzc&xh="
			<< uid
			<< "&xnxqid="
			<< semester
			<< "&zc="
			<< week;
		HTTP http;
		http.AddHeader("token", token_);
		auto resp = http.Get(url.str());
		if (!resp.Ready) throw runtime_error("请求无响应.");
		if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");
		json re_json = json::parse(resp.Content);
		vector<Course> res;
		// fuck 不走寻常路的json
		if (re_json.size() == 0 || re_json[0].is_null()) return res;
		res.reserve(re_json.size());
		for (auto&& ele : re_json)
		{
			unsigned cweek, lesson1, lesson2;
			string t = ele["kcsj"].get<string>();
			stringstream ss;
			ss.str(t.substr(0, 1));
			ss >> cweek;
			ss.str(t.substr(1, 2));
			ss >> lesson1;
			ss.str(t.substr(2, 2));
			ss >> lesson2;

			Course c;
			try
			{
				c.Name = ele["kcmc"].get<string>();
			}
			catch (...)
			{
				c.Name = "?";
			}
			try
			{
				c.Classroom = ele["jsmc"].get<string>();
			}
			catch (...)
			{
				c.Classroom = "?";
			}
			try
			{
				c.Instructor = ele["jsxm"].get<string>();
			}
			catch (...)
			{
				c.Instructor = "?";
			}
			try
			{
				c.StartTime = ele["kssj"].get<string>();
			}
			catch (...)
			{
				c.StartTime = "?";
			}
			try
			{
				c.EndTime = ele["jssj"].get<string>();
			}
			catch (...)
			{
				c.EndTime = "?";
			}
			try
			{
				c.Schedule = ele["kkzc"].get<string>();
			}
			catch (...)
			{
				c.Schedule = "?";
			}
			c.Week = cweek;
			c.StartLesson = lesson1;
			c.EndLesson = lesson2;
			res.push_back(c);
		}
		return res;
	}

	vector<Jw::ExamResult> Jw::GetExamResult(const string& uid, const string& semester)
	{
		if (token_.empty()) throw runtime_error("尚未登录.");
		stringstream url;
		url << api_prefix_
			<< "/app.do?method=getCjcx&xh="
			<< uid
			<< "&xnxqid="
			<< semester;
		HTTP http;
		http.AddHeader("token", token_);
		auto resp = http.Get(url.str());
		if (!resp.Ready) throw runtime_error("请求无响应.");
		if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");
		json re_json = json::parse(resp.Content);
		vector<ExamResult> res;
		// fuck 不走寻常路的json
		if (re_json.size() == 0 || re_json[0].is_null()) return res;
		res.reserve(re_json.size());
		for (auto&& ele : re_json)
		{
			ExamResult exam_result;
			try
			{
				exam_result.Name = ele["kcmc"].get<string>();
				exam_result.Score = ele["zcj"].get<string>();
				exam_result.Credit = ele["xf"].get<double>();
				exam_result.Semester = ele["xqmc"].get<string>();
				exam_result.CourseCategory = ele["kclbmc"].get<string>();
				exam_result.CourseNature = ele["kcxzmc"].get<string>();
			}
			catch (...)
			{
			}
			res.push_back(exam_result);
		}
		return res;
	}

	vector<Jw::ExamSchedule> Jw::GetExamSchedule(const string& uid)
	{
		stringstream url;
		url << api_prefix_
			<< "/app.do?method=getKscx&xh="
			<< uid;
		HTTP http;
		http.AddHeader("token", token_);
		auto resp = http.Get(url.str());
		if (!resp.Ready) throw runtime_error("请求无响应.");
		if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");
		json re_json = json::parse(resp.Content);
		vector<ExamSchedule> res;
		// fuck 不走寻常路的json
		if (re_json.size() == 0 || re_json[0].is_null()) return res;
		res.reserve(re_json.size());
		for (auto&& ele : re_json)
		{
			ExamSchedule es;
			es.CourseName = ele["kcmc"].get<string>();
			es.ExamRoom = ele["jsmc"].get<string>();
			es.ChineseTime = ele["vksjc"].get<string>();
			es.Time = ele["ksqssj"].get<string>();
			res.push_back(es);
		}
		return res;
	}

	string Jw::GetMajor(const string& uid)
	{
		stringstream url;
		url << api_prefix_
			<< "/app.do?method=getUserInfo&xh="
			<< uid;
		HTTP http;
		http.AddHeader("token", token_);
		auto resp = http.Get(url.str());
		if (!resp.Ready) throw runtime_error("请求无响应.");
		if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");
		json re_json = json::parse(resp.Content);
		return re_json["zymc"];
	}

	vector<Jw::FreeClassroom> Jw::GetFreeClassroom(const string& date, Jw::FreeTime freeTime, const string& campus_id, const string& building_id)
	{
		static string freetime[] = { "allday", "am", "pm", "night" };
		stringstream url;
		url << api_prefix_
			<< "/app.do?method=getKxJscx&time="
			<< date
			<< "&idleTime="
			<< freetime[(int)(freeTime)]
			<< "&xqid="
			<< campus_id
			<< "&jxlid="
			<< building_id;

		HTTP http;
		http.AddHeader("token", token_);
		auto resp = http.Get(url.str());
		if (!resp.Ready) throw runtime_error("请求无响应.");
		if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");
		json re_json = json::parse(resp.Content);
		vector<FreeClassroom> res;
		// fuck 不走寻常路的json
		if (re_json.is_object()) return res;
		res.reserve(re_json.size());
		for (auto&& ele : re_json[0]["jsList"])
		{
			FreeClassroom es;
			es.ClassroomName = ele["jsmc"].get<string>();
			es.CampusName = ele["xqmc"].get<string>();
			es.BuildingName = ele["jzwmc"].get<string>();
			es.Capacity = ele["zws"].get<int>();
			es.Floor = GetFloor(es.ClassroomName);
			res.push_back(es);
		}
		return res;

	}

	int Jw::GetFloor(const string& classroomName)
	{
		int result = -1;
		regex pattern(R"((?:A01|20|30|第二教学楼|\[)(\d)(?:\d\d(\D|$)|楼))", regex::ECMAScript);
		std::smatch matches;
		if (regex_search(classroomName, matches, pattern))
		{
			stringstream ss(matches.str(1));
			ss >> result;
		}
		return result;
	}

	vector<Jw::Weather> Jw::GetWeather()
	{
		const string SFURL = "http://www.weather.com.cn/weather/101040500.shtml";
		const string NAURL = "http://www.weather.com.cn/weather/101044000.shtml";

		vector<Weather> allWeather;

		vector<Weather> sfWeather = GetWeatherByUrl(SFURL, "SF");
		vector<Weather> naWeather = GetWeatherByUrl(SFURL, "NA");

		allWeather.insert(allWeather.end(), sfWeather.begin(), sfWeather.end());
		allWeather.insert(allWeather.end(), naWeather.begin(), naWeather.end());

		return allWeather;

	}

	vector<Jw::Weather> Jw::GetWeatherByUrl(const string& url, const string& pos)
	{
		vector<Weather> result;
		HTTP http;
		auto resp = http.Get(url);
		if (!resp.Ready) throw runtime_error("请求无响应");
		if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");

		HTMLDoc hdoc;
		hdoc.Parse(resp.Content);

		auto res = hdoc.Search(
			[](const string& tagName, const Attributes& attrs)
			{
				return (tagName == "ul" && attrs.Exist("class", "t clearfix"));
			});

		try {

			//只有一个ul
			for (auto& ul : res)
			{
				auto theLies = ul.SearchByTagName("li");

				for (auto& li : theLies)
				{
					Weather wea;
					wea.Position = pos;
					wea.Date = li["h1"].GetInner();

					auto ps = li.SearchByTagName("p");
					wea.Weather = ps[0].GetInner();
					wea.MinTemperature = ps[1]["i"].GetInner();

					//晚上时当前无最大温度
					auto maxTemH = ps[1].SearchByTagName("span");
					wea.MaxTemperature = (maxTemH.empty() ? "None" : maxTemH[0].GetInner());

					result.push_back(wea);
				}
			}

		}
		catch (const CantFindAttribute& cfa)
		{
			cout << cfa.what() << endl;
			hdoc.PrintTree(cout, true);
		}

		return result;

	}

	vector<Jw::WeatherOneDay> Jw::GetWeatherOneDay()
	{
		const string SFURL = "http://www.weather.com.cn/weather/101040500.shtml";
		const string NAURL = "http://www.weather.com.cn/weather/101044000.shtml";

		vector<WeatherOneDay> allWeatherOneDay;

		vector<WeatherOneDay> sfWeather = GetWeatherOneDayByUrl(SFURL, "SF");
		vector<WeatherOneDay> naWeather = GetWeatherOneDayByUrl(SFURL, "NA");

		allWeatherOneDay.insert(allWeatherOneDay.end(), sfWeather.begin(), sfWeather.end());
		allWeatherOneDay.insert(allWeatherOneDay.end(), naWeather.begin(), naWeather.end());

		return allWeatherOneDay;
	}

	vector<Jw::WeatherOneDay> Jw::GetWeatherOneDayByUrl(const string& url, const string& pos)
	{
		vector<WeatherOneDay> result;
		HTTP http;
		auto resp = http.Get(url);
		if (!resp.Ready) throw runtime_error("请求无响应");
		if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");

		string x = resp.Content;

		regex pattern(R"(observe24h_data *= *([^;]+);)");
		std::smatch matches;
		string theJsonText = "";

		if (regex_search(x, matches, pattern))
		{
			theJsonText = matches.str(1);
		}

		json weaJson = json::parse(theJsonText);
		json weaData = weaJson["od"]["od2"];
		string day = "tom";
		int i = 0; //判断今明天
		for (auto& ele : weaData)
		{
			WeatherOneDay wOne;
			wOne.Position = pos;
			wOne.Hour = ele["od21"].get<string>();
			if (day == "tom" && i != 0 && wOne.Hour == "23")
			{
				day = "today";
			}

			wOne.Day = day;
			wOne.Temperature = ele["od22"].get<string>();
			wOne.Precipitation = ele["od26"].get<string>();
			result.push_back(wOne);
			i++;
		}


		return result;
	}

}