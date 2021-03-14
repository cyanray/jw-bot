#include <iostream>
#include <sstream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdSubscribeNews(Message m)
{
	try
	{

		if (m.MessageChain.GetPlainTextFirst() == "订阅教务新闻")
		{
			if (UserDb.GetSid(m.Sender).empty())
			{
				m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
				return;
			}
			UserDb.UpdateNewsSubscription(m.Sender, 1);
			m.Reply(MessageChain().Plain("订阅成功，有新的教务新闻将会私聊发送给你（退订回复【取消订阅教务新闻】）"));
			return;
		}

		if (m.MessageChain.GetPlainTextFirst() == "取消订阅教务新闻")
		{
			if (UserDb.GetSid(m.Sender).empty())
			{
				m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
				return;
			}
			UserDb.UpdateNewsSubscription(m.Sender, 0);
			m.Reply(MessageChain().Plain("已取消订阅教务新闻"));
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