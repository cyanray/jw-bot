#include <iostream>
#include <sstream>
#include <iomanip>
#include "main.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CmdScore(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "查绩点") return;

	LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [查绩点] 指令";

	try
	{
		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}
		string schoolId = UserDb.GetSid(m.Sender);

		vector<string> semesters;
		semesters.reserve(8);
		int tYear;
		stringstream ss;
		ss << schoolId[2] << schoolId[3];
		ss >> tYear;
		// TODO: 该行代码将会在80年后出错 (2020-07-11)
		tYear += 2000;
		for (int i = 0; i < 4; i++)
		{
			stringstream ss;
			ss << tYear << '-' << tYear + 1 << "-1";
			semesters.push_back(ss.str());
			ss.str("");
			ss << tYear << '-' << tYear + 1 << "-2";
			semesters.push_back(ss.str());
			tYear++;
		}


		vector<cyanray::Jw::ExamResult> exam_results_tmp;
		vector<cyanray::Jw::ExamResult> exam_results_all;
		for (int i = 0; i < semesters.size(); ++i)
		{
			auto tmp = JwApi.GetExamResult(schoolId, semesters[i]);
			exam_results_tmp.insert(exam_results_tmp.end(), tmp.begin(), tmp.end());
			exam_results_all.insert(exam_results_all.end(), tmp.begin(), tmp.end());

			if (i % 2 || semesters[i] == GetThisSemester())
			{
				if (exam_results_tmp.empty())
				{
					m.Reply(MessageChain().Plain("没有查到你的成绩，你是否已被退学？"));
					return;
				}

				double GPA = 0, GPA_require_only = 0;
				CalcGPA(exam_results_tmp, GPA, GPA_require_only);

				stringstream ss1;
				ss1 << fixed << setprecision(2) << GPA_require_only;
				stringstream ss2;
				ss2 << fixed << setprecision(2) << GPA;

				MessageChain mc;
				mc.Plain(semesters[i - 1]).Plain(" ~ ").Plain(semesters[i]).Plain("\n")
					.Plain("平均学分绩点(排除校选)：【").Plain(ss1.str()).Plain("】\n")
					.Plain("平均学分绩点(所有科目)：【").Plain(ss2.str()).Plain("】");
				m.Reply(mc);
				exam_results_tmp.clear();
			}
			if (semesters[i] == GetThisSemester()) break;
		}

		double GPA = 0, GPA_require_only = 0;
		CalcGPA(exam_results_all, GPA, GPA_require_only);

		stringstream ss1;
		ss1 << fixed << setprecision(2) << GPA_require_only;
		stringstream ss2;
		ss2 << fixed << setprecision(2) << GPA;

		MessageChain mc;
		mc.Plain("【所有考试科目】\n")
			.Plain("平均学分绩点(排除校选)：【").Plain(ss1.str()).Plain("】\n")
			.Plain("平均学分绩点(所有科目)：【").Plain(ss2.str()).Plain("】");
		m.Reply(mc);

	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [查绩点] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [查绩点] 指令时出现异常: " << ex.what();
		}
	}
}
