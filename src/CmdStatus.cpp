#include <iostream>
#include <fmt/core.h>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdStatus(Cyan::Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "运行状态") return;

	try
	{
		MessageChain mc;
		mc.Plain("[课表机器人运行状态]\n");
		mc.Plain(fmt::format("总用户数量: {} 人\n", UserDb.GetUserCountAll()));

		int max_grade = GetYear() - 2000;	// TODO: 将会在100年后失效
		for (int grade = 17; grade <= max_grade; grade++)
		{
			mc.Plain(fmt::format("{} 级用户数量: {} 人\n", grade, UserDb.GetUserCount(grade)));
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