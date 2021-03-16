#include <iostream>
#include <sstream>
#include <string_view>
#include <glog/logging.h>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdRefreshCourses(Message m)
{
	string msg_str = m.MessageChain.GetPlainTextFirst();
	string_view msg_view(msg_str);

	if (!msg_view.starts_with("刷新课表")) return;

	try
	{
		LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [刷新课表] 指令";

		string schoolId = UserDb.GetSid(m.Sender);
		if (schoolId.empty())
		{
			LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [刷新课表] 指令时出现错误: 没有找到学号!";
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}

		UserDb.ClearCourses(m.Sender);

		for (int week_count = 1; week_count <= 20; week_count++)
		{
			LOG(INFO) << "[" << m.Sender.ToInt64() << "] 获取第 " << week_count << " 周的课表...";
			if (week_count % 5 == 1)
			{
				m.Reply(MessageChain()
					.Plain("正在获取第 ")
					.Plain(week_count).Plain("~").Plain(week_count + 4)
					.Plain(" 周的课表..."));
			}

			auto courses = JwApi.GetCourses(schoolId, week_count, GetThisSemester());
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
		LOG(INFO) << "[" << m.Sender.ToInt64() << "] 刷新课表成功!";
		m.Reply(MessageChain().Plain("刷新课表成功!"));
	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [刷新课表] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("获取课表失败, 请稍后重试！\n错误原因: \n").Plain(ex.what()));
		}
		catch (const std::exception& ex)
		{
			LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [刷新课表] 指令时出现异常: " << ex.what();
		}
	}


}