#include <iostream>
#include <sstream>
#include "main.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CmdSubscribeNews(Message m)
{
	try
	{

		if (m.MessageChain.GetPlainTextFirst() == "订阅教务新闻")
		{
			LOG(INFO) << "[" << m.Sender << "] 使用 [订阅教务新闻] 指令";

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
			LOG(INFO) << "[" << m.Sender << "] 使用 [取消订阅教务新闻] 指令";

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
		LOG(ERROR) << "[" << m.Sender << "] 使用 [(取消)订阅教务新闻] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用 [(取消)订阅教务新闻] 指令时出现异常: " << ex.what();
		}
	}
}