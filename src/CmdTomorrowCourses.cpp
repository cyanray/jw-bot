#include <iostream>
#include <sstream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdTomorrowCourses(Message m)
{
	string txt = m.MessageChain.GetPlainTextFirst();
	if (txt != "明天的" && txt != "明天课表" && txt != "明天的课表" && txt != "明日课表")
		return;
	try
	{
		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain("没有查到你的课表，回复 \"绑定学号\" 启动本功能"));
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
		auto today_courses = UserDb.GetCourses(m.Sender, week_of_semester, week);

		MessageChain mc;
		int count = today_courses.size();
		if (count > 0)
		{
			mc.Plain("你明天共有 ").Plain(count).Plain(" 节课\n");
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
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (...)
		{
		}
	}
}