#include "qzjw.h"
#include <sstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <CURLWrapper.h>
#include <regex>
#include <iostream>
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

}
