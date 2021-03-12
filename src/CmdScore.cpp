#include <iostream>
#include <sstream>
#include <iomanip>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdScore(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "查绩点") return;

	try
	{
		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain("🙄我还不知道你的学号! 回复 \"绑定学号+空格+学号\" 启动本功能"));
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


		vector<cyanray::Jw::ExamResult> res;
		for (int i = 0; i < semesters.size(); ++i)
		{
			auto tres = JwApi.GetExamResult(schoolId, semesters[i]);
			res.insert(res.end(), tres.begin(), tres.end());

			if (i % 2 || semesters[i] == GetThisSemester())
			{
				if (res.empty())
				{
					m.Reply(MessageChain().Plain("没有查到你的成绩，你是否已被退学？"));
					return;
				}

				double GPA = 0, credit_sum = 0;
				double GPA_require_only = 0, credit_sum_require_only = 0;
				for (auto&& exam : res)
				{
					// cout << exam.CourseCategory << endl;
					if (exam.CourseCategory != "校选")
					{
						double t = ScoreToGradePoint(exam.Score);
						GPA_require_only += t * exam.Credit;
						credit_sum_require_only += exam.Credit;
					}

					double t = ScoreToGradePoint(exam.Score);
					GPA += t * exam.Credit;
					credit_sum += exam.Credit;

				}

				GPA = GPA / credit_sum;
				GPA_require_only = GPA_require_only / credit_sum_require_only;

				stringstream ss1;
				ss1 << fixed << setprecision(2) << GPA_require_only;
				stringstream ss2;
				ss2 << fixed << setprecision(2) << GPA;

				MessageChain mc;
				mc.Plain(semesters[i - 1]).Plain(" ~ ").Plain(semesters[i]).Plain("\n")
					.Plain("平均学分绩点(排除校选)：【").Plain(ss1.str()).Plain("】\n")
					.Plain("平均学分绩点(所有科目)：【").Plain(ss2.str()).Plain("】");
				try
				{
					m.Reply(mc);
				}
				catch (...)
				{
				}

				res.clear();
			}
			if (semesters[i] == GetThisSemester()) break;
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