#include <iostream>
#include <sstream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdUnbinding(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "解除绑定学号") return;

	try
	{
		if (UserDb.GetSid(m.Sender).empty())
		{
			m.Reply(MessageChain().Plain("🙄我还不知道你的学号! 回复 \"绑定学号+空格+学号\" 启动本功能"));
			return;
		}
		UserDb.Delete(m.Sender);
		m.Reply(MessageChain().Plain("已经清除你的学号、课表信息"));
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