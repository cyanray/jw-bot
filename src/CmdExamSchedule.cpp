#include <iostream>
#include <sstream>
#include <glog/logging.h>
#include "main.h"
using namespace std;
using namespace Cyan;
using namespace cyanray;

void CmdExamSchedule(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "考试安排") return;
	LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [考试安排] 指令";

	string schoolId = UserDb.GetSid(m.Sender);
	if (schoolId.empty())
	{
		LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [考试安排] 指令时出现错误: 没有找到学号!";
		m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
		return;
	}

	try
	{
		LOG(INFO) << "[" << m.Sender.ToInt64() << "] 获取 [" << schoolId << "] 的考试安排...";
		vector<Jw::ExamSchedule> es = JwApi.GetExamSchedule(schoolId);
		LOG(INFO) << "[" << m.Sender.ToInt64() << "] 获取到 " << es.size() << " 个考试安排";
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
			LOG(INFO) << "[" << m.Sender.ToInt64() << "] 考试安排: " << item.CourseName << ", " << item.ExamRoom << ", " << item.ChineseTime;
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
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [考试安排] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [考试安排] 指令时出现异常: " << ex.what();
		}
	}

}