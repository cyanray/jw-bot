#include <iostream>
#include <sstream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdSubscribeMorning(Message m)
{
	try
	{
		if (m.MessageChain.GetPlainTextFirst() == "订阅每日课表")
		{
			if (UserDb.GetSid(m.Sender).empty())
			{
				m.Reply(MessageChain().Plain("🙄我还不知道你的学号! 回复 \"绑定学号+空格+学号\" 启动本功能"));
				return;
			}
			UserDb.UpdateMorningSubscription(m.Sender, 1);
			m.Reply(MessageChain().Plain("订阅成功，每天早晨你将会收到当天的课表。（退订回复【取消订阅每日课表】）"));
			return;
		}

		if (m.MessageChain.GetPlainTextFirst() == "取消订阅每日课表")
		{
			if (UserDb.GetSid(m.Sender).empty())
			{
				m.Reply(MessageChain().Plain("🙄我还不知道你的学号! 回复 \"绑定学号+空格+学号\" 启动本功能"));
				return;
			}
			UserDb.UpdateMorningSubscription(m.Sender, 0);
			m.Reply(MessageChain().Plain("已取消订阅每日课表"));
			return;
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