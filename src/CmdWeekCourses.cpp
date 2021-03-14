#include <iostream>
#include <sstream>
#include "main.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CmdWeekCourses(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "本周课表") return;

	LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [本周课表] 指令";

	const static string weekdayStr[7] = { "星期一","星期二","星期三","星期四","星期五","星期六","星期天" };
	try
	{
		// GetWeek()范围为[1,20]
		int week = GetWeekOfSemester();		// 获取本学期第几周

		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}

		m.Reply(MessageChain().Plain("本周是第 ").Plain(week).Plain(" 周"));
		for (int w = 1; w <= 7; ++w)
		{
			auto courses = UserDb.GetCourses(m.Sender, week, w);
			MessageChain mc;
			int count = courses.size();
			if (count > 0)
			{
				mc.Plain("本周 ").Plain(weekdayStr[w - 1]).Plain(" 你共有 ").Plain(count).Plain(" 节课\n");
				mc = mc + CoursesFormat(courses);
			}
			else
			{
				mc.Plain("本周 ").Plain(weekdayStr[w - 1]).Plain(" 你没有课");
			}
			m.Reply(mc);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [本周课表] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [本周课表] 指令时出现异常: " << ex.what();
		}
	}
}
