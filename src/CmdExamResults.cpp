#include <iostream>
#include <sstream>
#include <iomanip>
#include <fmt/core.h>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdExamResult(Message m)
{
	string msg_str = m.MessageChain.GetPlainTextFirst();
	string_view msg_view(msg_str);
	if (!msg_view.starts_with("查成绩")) return;

	try
	{
		string schoolId = UserDb.GetSid(m.Sender);
		if (schoolId.empty())
		{
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}

		if (msg_view.ends_with("!") || msg_view.ends_with("！"))
		{
			schoolId = UserDb.GetFriendSid(m.Sender);
		}
		if (schoolId.empty())
		{
			m.Reply(MessageChain().Plain("使用【交个朋友】指令建立好友关系后可以查询对方的绩点！"));
			return;
		}

		auto exam_results = JwApi.GetExamResult(schoolId, GetThisSemester());
		if (exam_results.empty())
		{
			exam_results = JwApi.GetExamResult(schoolId, GetLastSemester());
		}
		if (exam_results.empty())
		{
			m.Reply(MessageChain().Plain("没有查到你的成绩，你是否已被退学？"));
			return;
		}

		// 3个课程的成绩为1组
		int count = 0;
		MessageChain mc_exam_result;
		for (auto&& exam : exam_results)
		{
			mc_exam_result.Plain(
				fmt::format("{}, {}, 成绩: {}, 学分: {}", exam.Semester, exam.Name, exam.Score, exam.Credit));
			count++;
			if (count % 3 == 0)
			{
				m.Reply(mc_exam_result);
				mc_exam_result.Clear();
			}
			else
			{
				mc_exam_result.Plain("\n");
			}
		}
		// 发送还未发送的成绩
		if (mc_exam_result.Count() != 0)
		{
			m.Reply(mc_exam_result);
		}

		double GPA = 0, GPA_require_only = 0;
		CalcGPA(exam_results, GPA, GPA_require_only);

		MessageChain mc;
		mc.Plain(fmt::format("平均学分绩点(排除校选)：{:.3f}\n", GPA_require_only));
		mc.Plain(fmt::format("平均学分绩点(所有科目)：{:.3f}", GPA));
		m.Reply(mc);
	}
	catch (const std::exception& ex)
	{
		try
		{
			m.Reply(MessageChain().Plain("查询成绩时出现错误："s + ex.what()));
		}
		catch (...) {}
	}
}