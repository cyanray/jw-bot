#include <iostream>
#include <sstream>
#include "main.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CmdUnbinding(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "解除绑定学号") return;

	LOG(INFO) << "[" << m.Sender << "] 使用 [解除绑定学号] 指令";

	try
	{
		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}
		UserDb.Delete(m.Sender);
		m.Reply(MessageChain().Plain("已经清除你的学号、课表信息"));
	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender << "] 使用 [解除绑定学号] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用 [解除绑定学号] 指令时出现异常: " << ex.what();
		}
	}
}