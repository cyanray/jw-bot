#include <iostream>
#include <sstream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdUpdateCourses(GroupMessage e)
{
	MiraiBot& bot = e.GetMiraiBot();
	try
	{
		if (e.MessageChain.GetPlainTextFirst() != "/更新所有用户课表") return;
		if (e.Sender.QQ.ToInt64() != AppConfig["AdminQQ"].get<int64_t>())
		{
			e.Reply(MessageChain().Plain("没有权限"));
			return;
		}

		vector<QQ_t> users = UserDb.GetAllUser();
		for (auto user : users)
		{
			string sid = UserDb.GetSid(user);
			try
			{
				json courses_data = json::array();
				for (int week_count = 1; week_count <= 20; week_count++)
				{
					json courses_week = json::array();
					for (int i = 0; i < 7; ++i)
					{
						courses_week[i] = json::array();
					}
					auto courses = JwApi.GetCourses(sid, week_count, GetThisSemester());
					for (auto&& c : courses)
					{
						if (c.StartTime == "08:00")
						{
							c.StartTime = "08:20";
						}
						else if (c.StartTime == "10:00")
						{
							c.StartTime = "10:20";
						}

						if (c.EndTime == "09:40")
						{
							c.EndTime = "10:00";
						}
						else if (c.EndTime == "11:40")
						{
							c.EndTime = "12:00";
						}
						json aCourse;
						aCourse["Name"] = c.Name;
						aCourse["Classroom"] = c.Classroom;
						aCourse["Time"] = c.StartTime + "-" + c.EndTime;
						courses_week[static_cast<int64_t>(c.Week) - 1].push_back(aCourse);
					}
					courses_data.push_back(courses_week);
				}
				// TODO: UserDb.UpdateCourses(user, courses_data);
				try
				{
					bot.SendMessage(user,
						MessageChain()
						.Plain(sid).Plain(" 同学，课表机器人已自动更新你本学期的课表 (" + to_string((int64_t)time(0)) + ")"));
				}
				catch (...) {}
			}
			catch (const std::exception& ex_downloadCourses)
			{
				bot.SendMessage(user,
					MessageChain()
					.Plain(sid).Plain(" 同学，课表机器人在更新你下学期课表时出现错误: ")
					.Plain(ex_downloadCourses.what()).Plain("\n")
					.Plain("如果课表出现错误，请尝试重新绑定学号。"));
			}
			MiraiBot::SleepSeconds(10);
		}

	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;	// TODO: log
	}

}