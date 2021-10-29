#include <iostream>
#include <sstream>
#include "main.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CmdTomorrowCourses(Message m)
{
	string msg_str = m.MessageChain.GetPlainTextFirst();
	string_view msg_view(msg_str);
	if (!msg_view.starts_with("明天的") &&
		!msg_view.starts_with("明天课表") &&
		!msg_view.starts_with("明天的课表") &&
		!msg_view.starts_with("明日课表"))
	{
		return;
	}

	LOG(INFO) << "[" << m.Sender << "] 使用 [明日课表] 指令: " << msg_str;

	try
	{
		if (UserDb.GetSid(m.Sender).empty())
		{
			LOG(INFO) << "[" << m.Sender << "] 使用 [明日课表] 指令时出现错误: 没有找到学号!";
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}

		QQ_t qq = m.Sender;
		if (msg_view.ends_with("!") || msg_view.ends_with("！"))
		{
			qq = UserDb.GetFriendQQ(qq);
		}
		if (qq.ToInt64() == -1)
		{
			m.Reply(MessageChain().Plain("使用【交个朋友】指令建立好友关系后可以查询对方的课表！"));
			return;
		}

		// GetWeek()范围为[1,20]
		int week_of_semester = GetWeekOfSemester();		// 获取本学期第几周
		// 获取明天是星期几
		int week = GetWeekToday() + 1;
		// 如果在星期天使用明天课表，那么应该返回下周一的课表
		if (week > 7)
		{
			week = 1;
			week_of_semester = ( week_of_semester + 1) % 21;
		}
		auto today_courses = UserDb.GetCourses(qq, week_of_semester, week);

		MessageChain mc;
		int count = today_courses.size();
		if (count > 0)
		{
			mc.Plain(fmt::format("你明天共有 {} 节课\n", count));
			mc = mc + CoursesFormat(today_courses);
		}
		else
		{
			mc.Plain("你明天没有课了!");
		}

		m.Reply(mc);

	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender << "] 使用 [明日课表] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用 [明日课表] 指令时出现异常: " << ex.what();
		}
	}
}