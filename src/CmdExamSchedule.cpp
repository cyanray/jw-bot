#include <iostream>
#include <sstream>
#include "main.h"
using namespace std;
using namespace Cyan;
using namespace cyanray;

void CmdExamSchedule(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "考试安排") return;

	if (UserDb.GetSid(m.Sender).empty())
	{
		m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
		return;
	}
	string schoolId = UserDb.GetSid(m.Sender);

	try
	{
		vector<Jw::ExamSchedule> es = JwApi.GetExamSchedule(schoolId);
		MessageChain mc;
		if (!es.empty())
		{
			mc.Plain("查询到 ").Plain(es.size()).Plain(" 个考试安排\n");
		}
		else
		{
			mc.Plain("没有查询到考试安排！");
		}
		m.Reply(mc);
		int i = 1;
		for (const auto& item : es)
		{
			MessageChain mc;
			mc
				.Plain(i++).Plain(". ").Plain(item.CourseName).Plain("\n")
				.Plain("教室：").Plain(item.ExamRoom).Plain("\n")
				.Plain("时间: ").Plain(item.ChineseTime).Plain("\n")
				.Plain(item.Time);
			m.Reply(mc);
		}

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