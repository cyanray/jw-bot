#include <iostream>
#include <sstream>
#include <iomanip>
#include "main.h"
#include <glog/logging.h>
#include <fmt/core.h>
using namespace std;
using namespace Cyan;

void CmdScore(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "查绩点") return;

	LOG(INFO) << "[" << m.Sender << "] 使用 [查绩点] 指令";

	try
	{
		string schoolId = UserDb.GetSid(m.Sender);
		if (schoolId.empty())
		{
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}

		vector<string> semesters;
		semesters.reserve(8);
		int enroll_year;
		stringstream ss;
		ss << schoolId[2] << schoolId[3];
		ss >> enroll_year;
		// TODO: 该行代码将会在80年后出错 (2020-07-11)
		enroll_year += 2000;
		int this_year = GetYear();
		while(enroll_year != this_year)
		{
			semesters.push_back(fmt::format("{}-{}-1", enroll_year, enroll_year + 1));
			semesters.push_back(fmt::format("{}-{}-2", enroll_year, enroll_year + 1));
			enroll_year++;
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
					continue;
				}

				double GPA = 0, GPA_require_only = 0;
				CalcGPA(exam_results_tmp, GPA, GPA_require_only);

				MessageChain mc;
				mc.Plain(fmt::format("{} ~ {}\n", semesters[i - 1], semesters[i]));
				mc.Plain(fmt::format("平均学分绩点(排除校选)：{:.3f}\n", GPA_require_only));
				mc.Plain(fmt::format("平均学分绩点(所有科目)：{:.3f}", GPA));
				m.Reply(mc);

				exam_results_tmp.clear();
			}
			if (semesters[i] == GetThisSemester()) break;
		}

		double GPA = 0, GPA_require_only = 0;
		CalcGPA(exam_results_all, GPA, GPA_require_only);

		MessageChain mc;
		mc.Plain("【所有考试科目】\n");
		mc.Plain(fmt::format("平均学分绩点(排除校选)：{:.3f}\n", GPA_require_only));
		mc.Plain(fmt::format("平均学分绩点(所有科目)：{:.3f}", GPA));
		m.Reply(mc);
	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender << "] 使用 [查绩点] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用 [查绩点] 指令时出现异常: " << ex.what();
		}
	}
}
