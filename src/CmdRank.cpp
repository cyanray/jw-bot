#include <iostream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdRank(Cyan::Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "rank") return;

	m.Reply(MessageChain().Plain("本功能暂停使用!"));
	return;

	if (UserDb.GetSid(m.Sender).empty())
	{
		m.Reply(MessageChain().Plain("🙄我还不知道你的学号! 回复 \"绑定学号+空格+学号\" 启动本功能"));
		return;
	}
	string schoolId = UserDb.GetSid(m.Sender);
	if (schoolId[3] != '7' && schoolId[3] != '8' && schoolId[3] != '9')
	{
		m.Reply(MessageChain().Plain("暂无数据"));
		return;
	}
	try
	{
		string major = JwApi.GetMajor(schoolId);

		vector<string> sid;
		string sql_str;
		if (schoolId[3] == '8')
			sql_str = "select sid from rank_18_2 where major= ? order by grade_point DESC";
		else if (schoolId[3] == '9')
			sql_str = "select sid from rank_19_1 where major= ? order by grade_point DESC";
		else
			sql_str = "select sid from rank_17_3 where major= ? order by grade_point DESC";

		SQLite::Statement sql(RankDb, sql_str);
		sql.bind(1, major);
		while (sql.executeStep())
		{
			string c = sql.getColumn(0);
			sid.push_back(c);
		}
		if (sid.empty())
		{
			m.Reply(MessageChain().Plain("暂无数据"));
			return;
		}
		for (int i = 0; i < sid.size(); ++i)
		{
			if (sid[i] == schoolId)
			{
				m.Reply(MessageChain()
					.Plain("rank:")
					.Plain(i + 1).Plain("/").Plain(sid.size())
					.Plain("(本学年，仅供参考)"));
				return;
			}
		}
		m.Reply(MessageChain().Plain("未找到 rank"));
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