#include <iostream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdStatus(Cyan::Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "运行状态") return;

	try
	{
		auto mc = MessageChain().Plain("[课表机器人运行状态]\n");
		mc.Plain("总用户数量: ").Plain(UserDb.GetUserCountAll()).Plain(" 人\n");
		mc.Plain("用户数量(17): ").Plain(UserDb.GetUserCount_17()).Plain(" 人\n");
		mc.Plain("用户数量(18): ").Plain(UserDb.GetUserCount_18()).Plain(" 人\n");
		mc.Plain("用户数量(19): ").Plain(UserDb.GetUserCount_19()).Plain(" 人\n");
		mc.Plain("用户数量(20): ").Plain(UserDb.GetUserCount_20()).Plain(" 人");
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