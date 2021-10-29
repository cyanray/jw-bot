#include <iostream>
#include <sstream>
#include <fmt/core.h>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdLastWeekCourses(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "上周课表") return;
	const static string weekdayStr[7] = { "星期一","星期二","星期三","星期四","星期五","星期六","星期天" };
	try
	{
		// GetWeek()范围为[1,20]
		int week = GetWeekOfSemester() - 1;
		if (week <= 0) week = 1;

		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}

		m.Reply(MessageChain().Plain(fmt::format("上周是第 {} 周", week)));
		for (int w = 1; w <= 7; ++w)
		{
			auto courses = UserDb.GetCourses(m.Sender, week, w);
			MessageChain mc;
			int count = courses.size();

			if (count > 0)
			{
				mc.Plain(fmt::format("上周 {} 你共有 {} 节课\n", weekdayStr[w - 1], count));
				mc = mc + CoursesFormat(courses);
			}
			else
			{
				mc.Plain(fmt::format("上周 {} 你没有课\n", weekdayStr[w - 1]));
			}
			m.Reply(mc);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

	}
	catch (const std::exception& ex)
	{
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (...) {}
	}
}