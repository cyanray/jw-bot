#include <iostream>
#include <sstream>
#include <iomanip>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdExamResult(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "查成绩") return;

	try
	{
		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}
		string schoolId = UserDb.GetSid(m.Sender);

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
		int count = 0;
		MessageChain mc_exam_result;
		for (auto&& exam : exam_results)
		{
			mc_exam_result
				.Plain(exam.Semester).Plain(", ")
				.Plain(exam.Name).Plain(", 成绩: ")
				.Plain(exam.Score).Plain(", 学分: ")
				.Plain(exam.Credit);
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

		if (mc_exam_result.Count() != 0)
		{
			m.Reply(mc_exam_result);
		}

		double GPA = 0, GPA_require_only = 0;
		CalcGPA(exam_results, GPA, GPA_require_only);

		stringstream ss1;
		ss1 << fixed << setprecision(3) << GPA_require_only;
		stringstream ss2;
		ss2 << fixed << setprecision(3) << GPA;

		MessageChain mc;
		mc.Plain("平均学分绩点(排除校选)：").Plain(ss1.str()).Plain("\n");
		mc.Plain("平均学分绩点(所有科目)：").Plain(ss2.str());

		m.Reply(mc);
	}
	catch (const std::exception& ex)
	{
		try
		{
			m.Reply(MessageChain().Plain("查询成绩时出现错误："s + ex.what()));
		}
		catch (...)
		{
		}
	}
}