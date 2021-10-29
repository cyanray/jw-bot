#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <fmt/core.h>
#include "main.h"
#include "utils.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CmdNWeekCourses(Message m)
{
	string msg_str = m.MessageChain.GetPlainTextFirst();

	try
	{
		wstring msg_wstr = Cyan::Encoding::UTF8::to_wstring(msg_str);
		wregex pattern(L"第([1-9]|1[0-9]|20|[\u4e00-\u9fa5]{1,2})周课表");
		wsmatch match;
		regex_search(msg_wstr, match, pattern);

		if (match.size() < 2)
		{
			return;
		}

		LOG(INFO) << "[" << m.Sender << "] 使用 [第N周课表] 指令";


		wstring weekNumberStr = match[1].str();
		int week = 1;
		if (std::isdigit(weekNumberStr[0]))
		{
			week = std::stoi(weekNumberStr);
		}
		else
		{
			week = Cyan::Utils::ChineseNumberTo<int>(weekNumberStr);
		}

		if (week < 0 || week > 20) return;

		const static string weekdayStr[7] = { "星期一","星期二","星期三","星期四","星期五","星期六","星期天" };

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
				mc.Plain(fmt::format("第 {} 周 {} 你共有 {} 节课\n", week, weekdayStr[w - 1], count));
				mc = mc + CoursesFormat(courses);
			}
			else
			{
				mc.Plain(fmt::format("第 {} 周 {} 你没有课\n", week, weekdayStr[w - 1]));
			}
			m.Reply(mc);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender << "] 使用 [第N周课表] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用 [第N周课表] 指令时出现异常: " << ex.what();
		}
	}
}
