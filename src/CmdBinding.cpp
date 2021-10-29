#include <iostream>
#include <sstream>
#include <string_view>
#include <glog/logging.h>
#include "main.h"
#include <regex>
#include <fmt/core.h>
using namespace std;
using namespace Cyan;

void CmdBinding(Message m)
{
	string msg_str = m.MessageChain.GetPlainTextFirst();

	if (!string_view(msg_str).starts_with("绑定学号")) return;

	try
	{
		LOG(INFO) << "[" << m.Sender << "] 使用 [绑定学号] 指令";
		regex pattern(R"((绑定学号).*?(6[3|6]\d{10}))");
		smatch match;
		regex_search(msg_str, match, pattern);
		if (match.size() < 3)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用了不正确的 [绑定学号] 指令: " << msg_str;
			m.Reply(MessageChain().Plain("正确格式: 绑定学号+空格+学号\n例如：“绑定学号 631805010000”"));
			return;
		}

		string sid = match[2].str();

		if (!UserDb.Exist(m.Sender))
		{
			UserDb.Add(m.Sender, sid);
		}
		else
		{
			// 以前绑定过学号，那么不自动刷新课表
			// 可能是绑定别人的学号，查别人的成绩
			UserDb.UpdateSid(m.Sender, sid);
			LOG(INFO) << "[" << m.Sender.ToInt64() << "] 更新学号.";
			m.Reply(MessageChain().Plain("已经更新你的学号!\n请发送“刷新课表”以更新课表数据!"));
			return;
		}

		UserDb.ClearCourses(m.Sender);

		for (int week_count = 1; week_count <= 20; week_count++)
		{
			LOG(INFO) << "[" << m.Sender << "] 获取第 " << week_count << " 周的课表...";
			if (week_count % 5 == 1)
			{
				m.Reply(MessageChain().Plain(fmt::format("正在获取第 {}~{} 周的课表...", week_count, week_count + 4)));
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
		LOG(INFO) << "[" << m.Sender << "] 绑定学号成功!";
		m.Reply(MessageChain().Plain("绑定学号成功!"));
	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender << "] 使用 [绑定学号] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("绑定学号失败, 请稍后重试！\n错误原因: \n").Plain(ex.what()));
		}
		catch (const std::exception& ex)
		{
			LOG(INFO) << "[" << m.Sender << "] 使用 [绑定学号] 指令时出现异常: " << ex.what();
		}
	}


}