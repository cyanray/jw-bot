#include <iostream>
#include <sstream>
#include <string_view>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdBinding(Message m)
{
	string msg_str = m.MessageChain.GetPlainTextFirst();
	string_view msg_view(msg_str);

	if (!msg_view.starts_with("绑定学号")) return;

	try
	{

		regex pattern(R"((绑定学号).*?(63\d{10}))");
		smatch match;
		regex_search(msg_str, match, pattern);
		if (match.size() < 3)
		{
			m.Reply(MessageChain().Plain("正确格式: 绑定学号+空格+学号\n例如：“绑定学号 631805010000”"));
			return;
		}

		string sid = match[2].str();

		if (!UserDb.Exist(m.Sender))
			UserDb.Add(m.Sender, sid);
		else
			UserDb.UpdateSid(m.Sender, sid);

		UserDb.ClearCourses(m.Sender);

		for (int week_count = 1; week_count <= 20; week_count++)
		{
			if (week_count % 5 == 1)
			{
				m.Reply(MessageChain()
					.Plain("正在获取第 ")
					.Plain(week_count).Plain("~").Plain(week_count + 4)
					.Plain(" 周的课表..."));
			}

			auto courses = JwApi.GetCourses(sid, week_count, GetThisSemester());
			for (auto&& c : courses)
			{
				if (c.StartTime == "08:00")	// fu*k jw
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
				UserDb.InsertCourse(m.Sender, c.Name, c.Classroom, c.StartTime, c.EndTime, week_count, c.Week);
			}
		}

		m.Reply(MessageChain().Plain("绑定学号成功!"));
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;	// TODO: log
		try
		{
			m.Reply(MessageChain().Plain("绑定学号失败, 请稍后重试！\n错误原因: \n").Plain(ex.what()));
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;	// TODO: log
		}
	}


}