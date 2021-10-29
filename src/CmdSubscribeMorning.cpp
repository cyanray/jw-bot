#include <iostream>
#include <sstream>
#include "main.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CmdSubscribeMorning(Message m)
{
	try
	{
		if (m.MessageChain.GetPlainTextFirst() == "订阅每日课表")
		{
			LOG(INFO) << "[" << m.Sender << "] 使用 [订阅每日课表] 指令";

			if (UserDb.GetSid(m.Sender).empty())
			{
				m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
				return;
			}
			UserDb.UpdateMorningSubscription(m.Sender, 1);
			m.Reply(MessageChain().Plain("订阅成功，每天早晨你将会收到当天的课表。（退订回复【取消订阅每日课表】）"));
			return;
		}

		if (m.MessageChain.GetPlainTextFirst() == "取消订阅每日课表")
		{
			LOG(INFO) << "[" << m.Sender << "] 使用 [取消订阅每日课表] 指令";

			if (UserDb.GetSid(m.Sender).empty())
			{
				m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
				return;
			}
			UserDb.UpdateMorningSubscription(m.Sender, 0);
			m.Reply(MessageChain().Plain("已取消订阅每日课表"));
			return;
		}
	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender << "] 使用 [(取消)订阅每日课表] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用 [(取消)订阅每日课表] 指令时出现异常: " << ex.what();
		}
	}
}