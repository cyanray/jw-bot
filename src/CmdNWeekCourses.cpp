#include <iostream>
#include <sstream>
#include "main.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CmdNWeekCourses(Message m)
{
	string msg_str = m.MessageChain.GetPlainTextFirst();
	string_view msg_view(msg_str);

	try
	{
		regex pattern(R"(第([1-9]|1[0-9]|20)周课表)");
		smatch match;
		regex_search(msg_str, match, pattern);
		if (match.size() < 2)
		{
			return;
		}

		LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [第N周课表] 指令";

		int week_tmp = std::stoi(match[1].str());

		const static string weekdayStr[7] = { "星期一","星期二","星期三","星期四","星期五","星期六","星期天" };

		// GetWeek()范围为[1,20]
		int week = week_tmp;

		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}

		for (int w = 1; w <= 7; ++w)
		{
			auto courses = UserDb.GetCourses(m.Sender, week, w);
			MessageChain mc;
			int count = courses.size();
			if (count > 0)
			{
				mc.Plain("第 ").Plain(week).Plain(" 周 ").Plain(weekdayStr[w - 1]).Plain(" 你共有 ").Plain(count).Plain(" 节课\n");
				mc = mc + CoursesFormat(courses);
			}
			else
			{
				mc.Plain("第 ").Plain(week).Plain(" 周 ").Plain(weekdayStr[w - 1]).Plain(" 你没有课");
			}
			m.Reply(mc);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [第N周课表] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [第N周课表] 指令时出现异常: " << ex.what();
		}
	}
}
